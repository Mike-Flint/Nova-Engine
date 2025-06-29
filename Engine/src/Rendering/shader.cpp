#include "Shader.hpp" // Підключення заголовочного файлу класу Shader.
Shader::~Shader(){
    this->Delete();
}

void Shader::init(const char* vertexPath, const char* fragmentPath, const char* geometryPath){
    // Змінні для збереження коду вершинного та фрагментного шейдерів.
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    
    std::ifstream vShaderFile; // Файловий потік для читання вершинного шейдера.
    std::ifstream fShaderFile; // Файловий потік для читання фрагментного шейдера.
    std::ifstream gShaderFile;


    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (geometryPath != nullptr)
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    
    try {
        // Відкриття файлів з кодом вершинного та фрагментного шейдерів.
        vShaderFile.open(_basePath + std::string(vertexPath));
        fShaderFile.open(_basePath + std::string(fragmentPath));
        if (geometryPath != nullptr)
            gShaderFile.open(_basePath + std::string(geometryPath));
        std::stringstream vShaderStream, fShaderStream, gShaderStream;

        // Зчитування даних із файлів у текстові потоки.
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        if (geometryPath != nullptr)
            gShaderStream << gShaderFile.rdbuf();

        // Закриття файлів після зчитування.
        vShaderFile.close();
        fShaderFile.close();
        if (geometryPath != nullptr)
            gShaderFile.close();

        // Отримання коду шейдерів у вигляді рядків.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr)
            geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure&) {
        std::cout << "ERROR::SHADER::FILE_NOT::SUCCESFULLY_READ" << std::endl;
    }

    // Конвертація текстового коду шейдерів у формат, який приймає OpenGL.
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char *gShaderCode;
    if (geometryPath == nullptr)
        gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry; // Ідентифікатори шейдерів.

    // Створення та компіляція вершинного шейдера.
    vertex = glCreateShader(GL_VERTEX_SHADER); // Створення об'єкта вершинного шейдера.
    glShaderSource(vertex, 1, &vShaderCode, nullptr); // Прив'язка коду шейдера.
    glCompileShader(vertex); // Компіляція шейдера.
    checkCompileErrors(vertex, 'V'); // Перевірка на помилки компіляції.

    // Створення та компіляція фрагментного шейдера.
    fragment = glCreateShader(GL_FRAGMENT_SHADER); // Створення об'єкта фрагментного шейдера.
    glShaderSource(fragment, 1, &fShaderCode, nullptr); // Прив'язка коду шейдера.
    glCompileShader(fragment); // Компіляція шейдера.
    checkCompileErrors(fragment, 'F'); // Перевірка на помилки компіляції.

    if (geometryPath != nullptr){
        // Створення та компіляція фрагментного шейдера.
        geometry = glCreateShader(GL_GEOMETRY_SHADER); // Створення об'єкта фрагментного шейдера.
        glShaderSource(geometry, 1, &gShaderCode, nullptr); // Прив'язка коду шейдера.
        glCompileShader(geometry); // Компіляція шейдера.
        checkCompileErrors(geometry, 'G'); // Перевірка на помилки компіляції.
    }

    // Створення програми шейдерів.
    ID = glCreateProgram(); // Створення нового об'єкта програми.
    glAttachShader(ID, vertex); // Прив'язка вершинного шейдера до програми.
    glAttachShader(ID, fragment); // Прив'язка фрагментного шейдера до програми.
    if (geometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID); // Лінкування програми шейдерів.
    checkCompileErrors(ID, 'P'); // Перевірка на помилки лінкування.

    // Видалення об'єктів шейдерів після успішного лінкування програми.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    init(vertexPath, fragmentPath, geometryPath);
}

// Активація програми шейдерів.
void Shader::Activate() {
    glUseProgram(ID);
}

// Видалення програми шейдерів.
void Shader::Delete() {
    glDeleteProgram(ID);
}

// Встановлення значення типу bool для уніформної змінної.
void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value); // Зміна значення уніформної змінної.
}

// Встановлення значення типу int для уніформної змінної.
void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value); // Зміна значення уніформної змінної.
}
// Встановлення значення типу float для уніформної змінної.
void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value); // Зміна значення уніформної змінної.
}

void Shader::setVec3(const char* name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}


// Перевірка помилок компіляції або лінкування шейдерів.
void Shader::checkCompileErrors(unsigned int& shader, const char& type) {
    int success; // Статус успішності операції.
    char infoLog[1024]; // Лог для збереження помилки.

    
    if (type == 'V' || type == 'F' || type == 'G') { // Вершинний, фрагментний або геометричний шейдер.
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Перевірка статусу компіляції.
        if (!success) { // Якщо компіляція неуспішна.
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog); // Отримання лога помилки.
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else if (type == 'P') { // Програма.
        glGetProgramiv(shader, GL_LINK_STATUS, &success); // Перевірка статусу лінкування.
        if (!success) { // Якщо лінкування неуспішне.
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog); // Отримання лога помилки.
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        std::cout << "ERROR::UNKNOWN_SHADER_TYPE: " << type << std::endl;
    }
}


