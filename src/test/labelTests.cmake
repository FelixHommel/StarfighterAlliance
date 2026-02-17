# Tests that require OpenGL to complete need to be labeled with the 'OpenGL' label because they can't be run for Windows CI jobs
set_tests_properties(
    ResourceContextTest.LoadShaderSuccess
    ResourceContextTest.LoadTextureSuccess
    ResourceContextTest.LoadMultipleResourcesConcurrently
    ResourceContextTest.ClearResourceContext
    ShaderTest.SetFourSingleFloatValuesWithUse
    ShaderTest.SetFloatVector4ValueWithUse
    ShaderTest.SetFloatVector3ValueWithUse
    ShaderTest.SetFloatVector2ValueWithUse
    ShaderTest.SetSingleFloatingValueWithUse
    ShaderTest.SetSingleIntegerValueWithUse
    ShaderTest.SetMatrix4ValueWithUse
    ShaderTest.SetThreeSingleFloatValuesWithUse
    ShaderTest.SetTwoSingleFloatValuesWithUse
    ShaderTest.ShaderRAII
    ShaderTest.ShaderMoveConstructor
    ShaderTest.ShaderMoveAssignment
    ShaderTest.ShaderMoveAssignmentOnSameShader
    ShaderTest.ShaderUseActivatesProgram
    TextureTest.TextureRAII
    TextureTest.TextureMoveConstructor
    TextureTest.TextureMoveAssignment
    PROPERTIES LABELS OpenGL
)
