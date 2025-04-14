#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>
#include <vector>
#include <iostream> // Para std::ostream, y tipos básicos

// Enum para los niveles de la Taxonomia de Bloom
enum class NivelBloom {
    RECORDAR,
    ENTENDER,
    APLICAR,
    ANALIZAR,
    EVALUAR,
    CREAR,
    INVALIDO // Para manejo de errores o valor por defecto
};

// Función auxiliar para convertir NivelBloom a string (para mostrar)
std::string nivelBloomToString(NivelBloom nivel);

// Función auxiliar para convertir string a NivelBloom (para entrada de usuario)
NivelBloom stringToNivelBloom(const std::string& s);


// Enum para el tipo de pregunta
enum class TipoPregunta {
    OPCION_MULTIPLE,
    VERDADERO_FALSO,
    INVALIDO
};

// Función auxiliar para convertir TipoPregunta a string
std::string tipoPreguntaToString(TipoPregunta tipo);


class Pregunta {
private:
    int id;
    std::string enunciado;
    TipoPregunta tipo;
    std::vector<std::string> opciones;
    std::string respuestaCorrecta;
    NivelBloom nivelTaxonomico;
    int tiempoEstimado;
    int anioUltimoUso;
    static int nextId;

public:
    // Constructor
    Pregunta(const std::string& enunciado, TipoPregunta tipo,
             const std::vector<std::string>& opciones, // Vacio si es V/F
             const std::string& respuesta, NivelBloom nivel, int tiempo);

    // Getters
    int getId() const;
    std::string getEnunciado() const;
    TipoPregunta getTipo() const;
    std::vector<std::string> getOpciones() const;
    std::string getRespuestaCorrecta() const;
    NivelBloom getNivelTaxonomico() const;
    int getTiempoEstimado() const;
    int getAnioUltimoUso() const;

    // Setters
    void setEnunciado(const std::string& enunciado);
    void setOpciones(const std::vector<std::string>& opciones);
    void setRespuestaCorrecta(const std::string& respuesta);
    void setNivelTaxonomico(NivelBloom nivel);
    void setTiempoEstimado(int tiempo);
    void setAnioUltimoUso(int anio);

    // Metodo para mostrar la información de la pregunta
    void mostrar() const;

};

#endif // PREGUNTA_H
