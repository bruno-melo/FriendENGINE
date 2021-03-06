<img align="right" height="60" src="Documents/logo.png">

# FRIEND Engine Framework
> Functional Real-Time Interactive Endogenous Neuromodulation and Decoding (FRIEND) Engine Framework

D'Or Institute for Research and Education (IDOR) - Neuroinformatics Workgroup

* http://idor.org
* Other tools from our group: https://github.com/InstitutoDOr


## Docs

* [Getting Started](Documents/getting-started/README.md)   
* [Developer Documentation](Documents/developer/README.md)


---


FRIEND engine is a new implementation of the FRIEND software, a fMRI neurofeedback processing package that has been on the NITRC website since Oct 2013. Its aim is to create a cross-platform framework for a real time neurofeedback processing pipeline that permits interoperability with other applications (e.g.: games engines for the creation of virtual scenarios) and the inclusion of new processing methods through the use of plug-ins provided with the distribution or developed and shared by FRIEND engine users. 

The FRIEND engine framework consists of three main parts. The first one is the front end that is responsible for presenting the neurofeedback information to the subject. The second one is the core engine, a multithread application server that listens to a TCP/IP port for request commands issued by the front end and performs the associated command processing. The third one is the plug-in file, a dynamic library file (a .so file Linux file or a Mac OSX dylib file) that implements specific functions that are called internally by the engine at specific times during the pipeline. The engine is written in C++, whereas the front end can be written in any language by the user/researcher. They can execute on different machines as they communicate through a TCP/IP protocol. 

Third-party software is used: the FSL toolbox (http://fsl.fmrib.ox.ac.uk/fsl/fslwiki/), the Alglib library (http://www.alglib.net/) and SimpleINI (https://github.com/brofield/simpleini).

**Domain**: Clinical Neuroinformatics, MR, Computational Neuroscience

## Citation

If you use FRIEND Engine in published work, please cite our [paper](http://journal.frontiersin.org/article/10.3389/fnbeh.2015.00003/full):

_Basilio R, Garrido GJ, Sato JR, Hoefle S, Melo BRP, Pamplona FA, Zahn R and Moll J (2015) FRIEND Engine Framework: a real time neurofeedback client-server system for neuroimaging studies. Front. Behav. Neurosci. 9:3. doi: 10.3389/fnbeh.2015.00003_
