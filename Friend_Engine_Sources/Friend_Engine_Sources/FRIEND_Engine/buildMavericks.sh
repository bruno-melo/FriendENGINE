g++ -w -c -x c++ \
main.cpp filefuncs.cpp ../PlugIn/vardb.cpp socket.cxx socket2.cpp parser.cpp intervals.cpp fslfuncs.cpp process.cpp engine.cpp defs.cpp PlugInHandler.cpp  confusionmatrix.cpp session.cpp \
-DUNIX -DDARWIN -DEXPOSE_TREACHEROUS -DHAVE_LIBPNG -DHAVE_ZLIB \
-I. \
-I../PlugIn \
-I$FSLDIR/src \
-I$FSLDIR/extras/src/newmat \
-I$FSLDIR/extras/src/libgd \
-I$FSLDIR/extras/src/libgdc \
-I$FSLDIR/extras/src/libpng \
-I$FSLDIR/extras/src/libprob \
-I../simpleini \
-L$FSLDIR/extras/lib \
-L$FSLDIR/lib \
-lfslio \
-lnewimage \
-lmiscmaths \
-lcprob \
-lnewmat \
-lniftiio \
-lz \
-lznz \
-lmiscplot \
-lgdc \
-lgd \
-lpng \
-lpthread -ldl

g++ -o ../Application/engine main.o process.o engine.o PlugInHandler.o confusionmatrix.o session.o vardb.o  intervals.o socket.o socket2.o fslfuncs.o filefuncs.o defs.o parser.o \
-L../libFiles \
-L$FSLDIR/extras/lib \
-L$FSLDIR/lib \
-lm \
-lfslio \
-lnewimage \
-lmiscmaths \
-lcprob \
-lprob \
-lnewmat \
-lniftiio \
-lz \
-lznz \
-lmiscplot \
-lgdc \
-lgd \
-lpng