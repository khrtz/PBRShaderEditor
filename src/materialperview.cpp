#include "materialpreview.h"
#include <QOpenGLShader>
#include <QVector3D>

MaterialPreview::MaterialPreview(QWidget *parent)
    : QOpenGLWidget(parent), program(nullptr), roughness(0.5f), metallic(0.5f) {}

MaterialPreview::~MaterialPreview() {
    delete program;
}

void MaterialPreview::initializeGL() {
    initializeOpenGLFunctions();
    program = new QOpenGLShaderProgram(this);

    const char *vertexShaderCode = R"(
    #version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
out vec3 FragPos;
out vec3 Normal;
void main() {
FragPos = aPos;
Normal = aNormal;
gl_Position = vec4(aPos, 1.0);
})";
const char *fragmentShaderCode = R"(
#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 camPos;
uniform float roughness;
uniform float metallic;
uniform vec3 lightPos;
uniform vec3 lightColor;
const float PI = 3.14159265359;
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}
vec3 cookTorranceBRDF(vec3 N, vec3 V, vec3 L, vec3 F0, float roughness, float metallic) {
    vec3 H = normalize(V + L);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    vec3 nominator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = nominator / denominator;
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);
    return (kD * lightColor / PI + specular) * lightColor * NdotL;
}
void main() {
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, vec3(1.0), metallic);
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 radiance = cookTorranceBRDF(N, V, L, F0, roughness, metallic);
    FragColor = vec4(radiance, 1.0);
})";

program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderCode);
program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderCode);
program->link();
}

void MaterialPreview::paintGL() {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
if (program && program->isLinked()) {
    program->bind();

    program->setUniformValue("roughness", roughness);
    program->setUniformValue("metallic", metallic);
    program->setUniformValue("camPos", QVector3D(0.0, 0.0, 3.0));
    program->setUniformValue("lightPos", QVector3D(10.0, 10.0, 10.0));
    program->setUniformValue("lightColor", QVector3D(1.0, 1.0, 1.0));

    // 描画処理
    GLfloat vertices[] = {
        // positions         // normals
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    program->release();
}

}

void MaterialPreview::resizeGL(int w, int h) {
glViewport(0, 0, w, h);
}

void MaterialPreview::updateMaterial(float roughness, float metallic) {
this->roughness = roughness;
this->metallic = metallic;
update();
}
