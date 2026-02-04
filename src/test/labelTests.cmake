# Tests that require OpenGL to complete need to be labeled with the 'OpenGL' label because they can't be run for Windows CI jobs
set_tests_properties(
    ShaderTest.ShaderRAII
    ShaderTest.ShaderMoveConstructor
    ShaderTest.ShaderMoveAssignment
    ShaderTest.ShaderUseActivatesProgram
    TextureTest.TextureRAII
    TextureTest.TextureMoveConstructor
    TextureTest.TextureMoveAssignment
    TextureTest.TextureBindActivatesTheTexture
    PROPERTIES LABELS OpenGL
)
