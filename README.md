2022.11.11
利用mfem软件读入网格文件，然后存为AFEPack的内部文件格式。
由于AFEPack自带的gmsh网格文件导入工具不十分好用，故写了各种转换工具。
目前测试，只要是mfem可以识别的网格文件，都可以转换，包括2d和3d网格。

