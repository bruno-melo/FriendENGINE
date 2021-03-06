//  
//  point_list.h
//
// Jesper Andersson, FMRIB Image Analysis Group
//
// Copyright (C) 2008-2012 University of Oxford 
//
/*  Part of FSL - FMRIB's Software Library
    http://www.fmrib.ox.ac.uk/fsl
    fsl@fmrib.ox.ac.uk
    
    Developed at FMRIB (Oxford Centre for Functional Magnetic Resonance
    Imaging of the Brain), Department of Clinical Neurology, Oxford
    University, Oxford, UK
    
    
    LICENCE
    
    FMRIB Software Library, Release 5.0 (c) 2012, The University of
    Oxford (the "Software")
    
    The Software remains the property of the University of Oxford ("the
    University").
    
    The Software is distributed "AS IS" under this Licence solely for
    non-commercial use in the hope that it will be useful, but in order
    that the University as a charitable foundation protects its assets for
    the benefit of its educational and research purposes, the University
    makes clear that no condition is made or to be implied, nor is any
    warranty given or to be implied, as to the accuracy of the Software,
    or that it will be suitable for any particular purpose or for use
    under any specific conditions. Furthermore, the University disclaims
    all responsibility for the use which is made of the Software. It
    further disclaims any liability for the outcomes arising from using
    the Software.
    
    The Licensee agrees to indemnify the University and hold the
    University harmless from and against any and all claims, damages and
    liabilities asserted by third parties (including claims for
    negligence) which arise directly or indirectly from the use of the
    Software or the sale of any products based on the Software.
    
    No part of the Software may be reproduced, modified, transmitted or
    transferred in any form or by any means, electronic or mechanical,
    without the express permission of the University. The permission of
    the University is not required if the said reproduction, modification,
    transmission or transference is done without financial return, the
    conditions of this Licence are imposed upon the receiver of the
    product, and all original and amended source code is included in any
    transmitted product. You may be held legally responsible for any
    copyright infringement that is caused or encouraged by your failure to
    abide by these terms and conditions.
    
    You are not permitted under this Licence to use this Software
    commercially. Use for which any financial return is received shall be
    defined as commercial use, and includes (1) integration of all or part
    of the source code or the Software into a product for sale or license
    by or on behalf of Licensee to third parties or (2) use of the
    Software or any derivative of it for research with the final aim of
    developing software products for sale or license to a third party or
    (3) use of the Software or any derivative of it for research with the
    final aim of developing non-software products for sale or license to a
    third party, or (4) use of the Software to provide any service to an
    external organisation for which payment is received. If you are
    interested in using the Software commercially, please contact Isis
    Innovation Limited ("Isis"), the technology transfer company of the
    University, to negotiate a licence. Contact details are:
    innovation@isis.ox.ac.uk quoting reference DE/9564. */
#ifndef point_list_h
#define point_list_h

#include <string>
#include <newmat.h>

namespace NEWIMAGE {

class PointListException: public std::exception
{
private:
  std::string m_msg;
public:
  PointListException(const std::string& msg) throw(): m_msg(msg) {}

  virtual const char * what() const throw() {
    return string("PointList:: msg=" + m_msg).c_str();
  }

  ~PointListException() throw() {}
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// The PointList is a class that contains a list of points in a 3D-space. A PointList
// is constructed by reading a file that may be
//
// 1. A first output file
// 2. A text file with a coordinate triplet on each row/column.
// 
// The PointList will also be responsible for translating from the native formats of
// these files into "mm-coordinates", which is what we will ask PointList to output
// for us. Hence the constructor for each of those cases will also take an image
// file that defines the space within which the points are defined.
//
///////////////////////////////////////////////////////////////////////////////////////////////

class PointList
{
public:
  PointList(const std::string& pfname, const std::string& ifname);
  unsigned int NPoints() const {return(static_cast<unsigned int>(_points.Ncols()));}
  NEWMAT::ReturnMatrix Point(unsigned int indx, bool one_ext=false) const;
  NEWMAT::ReturnMatrix RawPoint(unsigned int indx, bool one_ext=false) const;
  NEWMAT::ReturnMatrix Vox2mm() const;
  NEWMAT::ReturnMatrix mm2Vox() const;
  void SetAffine(const NEWMAT::Matrix& aff);

private:
  int read_ascii_file(const std::string& fname);
  int read_first_file(const std::string& fname);
  bool is_maybe_vtk(const std::string& fname) const;

  std::string       _pfname;   // Name of file containing points
  std::string       _ifname;   // Name of image file defining space
  NEWMAT::Matrix    _SQform;   // qform/sform from image file
  NEWMAT::Matrix    _vox2mm;   // Voxel->mm-coordinates transform of _ifname
  NEWMAT::Matrix    _affine;   // Optional affine transform
  NEWMAT::Matrix    _points;   // Points in mm-coordinates of _ifname
}; 

} // End namespace NEWIMAGE

#endif // End #ifndef point_list_h
