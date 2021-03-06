//
//  svmPlugIn.cpp
//  
//
//  Created by IDOR on 06/06/13.
//
//

#include "MotorPlugIn.h"
#include "session.h"

#ifdef WINDOWS
#define DLLExport extern "C" __declspec(dllexport) int 
#else
#define DLLExport extern "C" int 
#endif

// initializes the object variables. This function brings the mni mask to subject space
int MotorRoiProcessing::initialization(studyParams &vdb)
{
   // getting roi intensities
   firstRoi = vdb.readedIni.GetLongValue("FRIEND", "firstROI");
   secondRoi = vdb.readedIni.GetLongValue("FRIEND", "secondROI");


   strcpy(vdb.mniMask, vdb.readedIni.GetValue("FRIEND", "MNIMask"));
   strcpy(vdb.mniTemplate, vdb.readedIni.GetValue("FRIEND", "MNITemplate"));
   int masktype = vdb.readedIni.GetLongValue("FRIEND", "ActivationLevelMaskType");

   fprintf(stderr, "mnimask = %s\n", vdb.mniMask);
   fprintf(stderr, "mnitemp = %s\n", vdb.mniTemplate);
   fprintf(stderr, "masktype = %d\n", masktype);

   targetValue = vdb.readedIni.GetDoubleValue("FRIEND", "ActivationLevel");
   // if exists the two files, bring  the mnimask to native space 
   if ((fileExists(vdb.mniMask)) && (fileExists(vdb.mniTemplate)) && (masktype == 2))
   {
      char outputFile[500], prefix[500]="_RFI2", name[500];
      
      extractFileName(vdb.mniMask, name);
      for (int t=0;t<strlen(name);t++)
      if (name[t] == '.') name[t] = '_';
      
      sprintf(outputFile, "%s%s%s.nii", vdb.inputDir, name, vdb.trainFeatureSuffix);
      
      fprintf(stderr, "Calculating the native template %s\n", outputFile);
      // bringing mni mask to subject space

	  // bet Functional
	  if (!fileExists(vdb.maskFile))
	  {
		  char CmdLn[1024];
		  sprintf(CmdLn, "bet %s %s %s", vdb.rfiFile, vdb.maskFile, vdb.betParameters);
		  bet(CmdLn);

		  sprintf(CmdLn, "bet %s %s %s", vdb.maskFile, vdb.maskFile, vdb.betParameters);
		  bet(CmdLn);
	  }
	  // bringing mni mask to subject space - using betted RFI file (RFI_sks.nii)
	  // MniToSubject(vdb.rfiFile, vdb.mniMask, vdb.mniTemplate, outputFile, prefix);
	  MniToSubject(vdb.maskFile, vdb.mniMask, vdb.mniTemplate, outputFile, prefix);

      // loads the reference mask
      meanCalculation.loadReference(outputFile);

	  // Theses indexes are the indexes relative to the roi intensities in the meanCalculation object mean array
	  firstRoiIndex  = meanCalculation.roiIndex(firstRoi);
	  secondRoiIndex = meanCalculation.roiIndex(secondRoi);
   }
   else if ((fileExists(vdb.mniMask)) && (masktype == 1))
   {
	   // loads the reference mask
	   fprintf(stderr, "Loading native space mask %s\n", vdb.mniMask);
	   meanCalculation.loadReference(vdb.mniMask);

	   // Theses indexes are the indexes relative to the roi intensities in the meanCalculation object mean array
	   firstRoiIndex = meanCalculation.roiIndex(firstRoi);
	   secondRoiIndex = meanCalculation.roiIndex(secondRoi);
   }
   else
   {
	   if (!fileExists(vdb.mniMask))
		   fprintf(stderr, "!!!!!!!   File not found %s.\n", vdb.mniMask);

	   if (!fileExists(vdb.mniTemplate))
		   fprintf(stderr, "!!!!!!!   File not found %s.\n", vdb.mniTemplate);
   };

   firstBaselineValue=0;
   secondBaselineValue = 0;
   return 0;
}

// calculates the feedback value
int MotorRoiProcessing::processVolume(studyParams &vdb, int index, float &classnum, float &feedbackValue)
{
   char processedFile[200];
   int idxInterval = vdb.interval.returnInterval(index);
   float firstRoiMean, secondRoiMean;
   float firstRoiFeedbackValue, secondRoiFeedbackValue;
   // the first roi feedback value goes directly through the normal channel, but the second roi need another route
   char secondRoiString[100];
   Session *session = (Session *)vdb.sessionPointer;

   volume<float> v;
   // gets the motion corrected and gaussian smoothed file
   vdb.getMCGVolumeName(processedFile, index);
   read_volume(v, string(processedFile));

   classnum = vdb.getClass(index);
   feedbackValue = 0;

   // if in baseline condition, calculates the mean volume
   if (vdb.interval.isBaselineCondition(index))
   {
      if (vdb.interval.intervals[idxInterval].start == index) meanbaseline = v;
      else meanbaseline += v;
      // in the end of condition, divides the sum by the size of the current block
      if (vdb.interval.intervals[idxInterval].end == index) 
      {
         meanbaseline /= (vdb.interval.intervals[idxInterval].end-vdb.interval.intervals[idxInterval].start+1);
         meanCalculation.calculateMeans(meanbaseline);
         
         // calculates the mean roi values of the mean volume. We just need this values
		 firstBaselineValue = meanCalculation.roiMean(firstRoiIndex);
		 secondBaselineValue = meanCalculation.roiMean(secondRoiIndex);
	  }

	  sprintf(secondRoiString, "0.0");
	  fprintf(stderr, "%s\n", secondRoiString);
	  session->processAdditionalFeedBackInfo(index, secondRoiString);
   }
   else // task condition. Taking the means of the rois in volume and calculating the PSC
   {
      meanCalculation.calculateMeans(v);
	  firstRoiMean = meanCalculation.roiMean(firstRoiIndex);
	  secondRoiMean = meanCalculation.roiMean(secondRoiIndex);

	  firstRoiFeedbackValue = PSC(firstRoiMean, firstBaselineValue) / targetValue;
	  secondRoiFeedbackValue = PSC(secondRoiMean, secondBaselineValue) / targetValue;
	  feedbackValue = firstRoiFeedbackValue;

	  sprintf(secondRoiString, "%f", secondRoiFeedbackValue);
	  session->processAdditionalFeedBackInfo(index, secondRoiString);

      // enforcing 0..1 range
      //if (projection > 1) projection = 1;
      //else if (projection < 0) projection = 0;
	  fprintf(stderr, "Feedback value = %f\n", feedbackValue);
	  fprintf(stderr, "Feedback value = %s\n", secondRoiString);
   }
   return 0;
}

// just calculate the percent signal change value
float MotorRoiProcessing::PSC(float value, float base)
{
   return (base) ? ((value-base) / base) : 0;
}

// plugin function for initializating the roi processing object
DLLExport initializeMotorProcessing(studyParams &vdb, void *&userData)
{
   MotorRoiProcessing *roiVar = new MotorRoiProcessing;
   roiVar->initialization(vdb);
   userData = roiVar;
   return 0;
}

// plugin function for finalizing the object
DLLExport finalizeMotorProcessing(studyParams &vdb, void *&userData)
{
   MotorRoiProcessing *roiVar = (MotorRoiProcessing *) userData;
   delete roiVar;
   return 0;
}

// plugin function for calculationg feedback value
DLLExport processMotorROI(studyParams &vdb, int index, float &classnum, float &projection, void * &userData)
{
   MotorRoiProcessing *roiVar = (MotorRoiProcessing *) userData;
   roiVar->processVolume(vdb, index, classnum, projection);
   return 0;
}
