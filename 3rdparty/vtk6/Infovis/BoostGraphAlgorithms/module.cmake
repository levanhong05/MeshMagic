vtk_module(vtkInfovisBoostGraphAlgorithms
  DEPENDS
    vtkInfovisCore
    vtkCommonExecutionModel
  TEST_DEPENDS
    vtkRenderingContext${VTK_RENDERING_BACKEND}
    vtkTestingRendering
    vtkInteractionStyle
    vtkIOInfovis
    vtkViewsInfovis
    vtkChartsCore
    vtkViewsContext2D
  )
