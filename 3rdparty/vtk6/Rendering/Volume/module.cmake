vtk_module(vtkRenderingVolume
  GROUPS
    Rendering
  DEPENDS
    vtkImagingCore
    vtkRenderingCore
  TEST_DEPENDS
    vtkFiltersModeling
    vtkTestingCore
    vtkTestingRendering
    vtkRenderingVolume${VTK_RENDERING_BACKEND}
    vtkRenderingFreeType
    vtkIOXML
    vtkImagingSources
    vtkImagingGeneral
    vtkImagingHybrid
    vtkInteractionStyle
    vtkIOLegacy
  KIT
    vtkRendering
  )
