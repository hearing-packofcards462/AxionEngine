// class ShaderRegistry final
// {
// public:
//     ShaderRegistry();
//     ~ShaderRegistry();

//     // Inicializa el compilador. Se llama al inicio del motor.
//     void init();
//     void shutdown();
    
//     // --- Registro y Compilación ---

//     // Registra, compila y extrae la reflexión de un archivo shader.
//     ShaderHandle registerShader( const SlangCompileDesc& desc, const std::string& name );

//     // --- Acceso ---
    
//     // Obtiene el bytecode compilado para el RHI
//     const std::vector<uchar>& getBytecode( ShaderHandle handle ) const;
    
//     // Obtiene la descripción de recursos para construir el Pipeline
//     const ShaderLayoutDesc& getLayout( ShaderHandle handle ) const;
    
//     // Búsqueda por nombre (para la carga inicial)
//     std::optional<ShaderHandle> findShader( const std::string& name ) const;
    
// private:
//     // Estructura interna para almacenar el recurso shader
//     struct ShaderRecord 
//     {
//         std::vector<uchar> bytecode;
//         ShaderLayoutDesc   layout;
//         std::string        name;
//         bool               alive = false;
//     };

//     // Componentes:
//     SlangShaderCompiler _compiler;
//     std::mutex          _mutex;

//     // Almacenamiento Interno (usa vector + mapa de nombres):
//     std::vector<ShaderRecord>                   _shaders;
//     std::unordered_map<std::string, ShaderHandle> _nameToHandle;

//     // Utilidad:
//     ShaderLayoutDesc _extractLayout( slang::IComponentType* program ) const;
// };

// } // namespace Graphics
// // AXION_NAMESPACE_END