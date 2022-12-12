# SC
## Building the project
***NOTE***: you can use the ```-j<n>``` option in any of the Makefiles to use more threads to compile faster (n stands for number of threads)

First you need to build the libraries in the external folder using
```shell
make MT
```

Then just run the vscode task to build one of the projects (SandBox or SCEditor)

***NOTE***: 
- You can run project by going into the debug tab and selecting the project to run and pressing F5
- MT is just enables multithread compiling and it is optional you can use ```make``` and it will compile fine
## Features
SC is not fully complete but has a lot of features:
- Batch Renderer
- Physics System based on box2d
- Custom shaders
- A FileSystem and Resource management system
- ApplicationAddons allows to create a overlay or add engine features (*Not stable*)
- Editor Application (*Not stable and **Experimental***)
## Optimizations
- Minimal use of heap memory
- Batch Renderer (*It Reduces the draw calls*)
