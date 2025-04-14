#include "pregunta.h" // Incluye la declaración de la clase Pregunta y los enums
#include <iostream>    // Para std::cout, std::endl, std::cerr
#include <string>      // Para std::string
#include <vector>      // Para std::vector
#include <stdexcept>   // Para std::invalid_argument (opcional, para validación más estricta)
#include <algorithm>   // Para std::transform
#include <cctype>      // Para ::tolower

// Implementación de Funciones Auxiliares

// Convierte NivelBloom a string para mostrar
std::string nivelBloomToString(NivelBloom nivel) {
    switch (nivel) {
        case NivelBloom::RECORDAR: return "Recordar";
        case NivelBloom::ENTENDER: return "Entender";
        case NivelBloom::APLICAR:  return "Aplicar";
        case NivelBloom::ANALIZAR: return "Analizar";
        case NivelBloom::EVALUAR:  return "Evaluar";
        case NivelBloom::CREAR:    return "Crear";
        default:                   return "Invalido";
    }
}

// Convierte string a NivelBloom
NivelBloom stringToNivelBloom(const std::string& s) {
    std::string lower_s = s;
    std::transform(lower_s.begin(), lower_s.end(), lower_s.begin(), ::tolower);

    if (lower_s == "recordar") return NivelBloom::RECORDAR;
    if (lower_s == "entender") return NivelBloom::ENTENDER;
    if (lower_s == "aplicar") return NivelBloom::APLICAR;
    if (lower_s == "analizar") return NivelBloom::ANALIZAR;
    if (lower_s == "evaluar") return NivelBloom::EVALUAR;
    if (lower_s == "crear") return NivelBloom::CREAR;
    return NivelBloom::INVALIDO;
}


// Convierte TipoPregunta a string para mostrar
std::string tipoPreguntaToString(TipoPregunta tipo) {
    switch (tipo) {
        case TipoPregunta::OPCION_MULTIPLE: return "Opcion Multiple";
        case TipoPregunta::VERDADERO_FALSO: return "Verdadero/Falso";
        default:                            return "Invalido";
    }
}

// Inicializacion del miembro estatico
int Pregunta::nextId = 1; // El primer ID será 1

// Implementacion de Métodos de la Clase Pregunta

// Constructor
Pregunta::Pregunta(const std::string& enunc, TipoPregunta tip,
                   const std::vector<std::string>& opts,
                   const std::string& resp, NivelBloom niv, int tiem)
    : id(nextId++), // Asigna el id actual y luego incrementa nextId para el siguiente
      enunciado(enunc),
      tipo(tip),
      opciones(opts),
      respuestaCorrecta(resp),
      nivelTaxonomico(niv),
      tiempoEstimado(tiem > 0 ? tiem : 1), // Asegura tiempo > 0 (minimo 1 min)
      anioUltimoUso(0) // Nueva pregunta, nunca usada
{
    // Validacion basica: si es V/F, las opciones deberian estar vacias.
    if (tipo == TipoPregunta::VERDADERO_FALSO && !opciones.empty()) {
         std::cerr << "Advertencia (Pregunta ID " << id << "): Se proporcionaron opciones para una pregunta Verdadero/Falso. Serán ignoradas." << std::endl;
         this->opciones.clear(); // Limpia las opciones si pasaron por error
    }
     // Validacion basica: si es opcion multiple, debe tener opciones.
     if (tipo == TipoPregunta::OPCION_MULTIPLE && opciones.empty()) {
         std::cerr << "Advertencia (Pregunta ID " << id << "): No se proporcionaron opciones para una pregunta de Opción Múltiple." << std::endl;
     }
}

// Getters
int Pregunta::getId() const {
    return id;
}

std::string Pregunta::getEnunciado() const {
    return enunciado;
}

TipoPregunta Pregunta::getTipo() const {
    return tipo;
}

std::vector<std::string> Pregunta::getOpciones() const {
    return opciones;
}

std::string Pregunta::getRespuestaCorrecta() const {
    return respuestaCorrecta;
}

NivelBloom Pregunta::getNivelTaxonomico() const {
    return nivelTaxonomico;
}

int Pregunta::getTiempoEstimado() const {
    return tiempoEstimado;
}

int Pregunta::getAnioUltimoUso() const {
    return anioUltimoUso;
}

// Setters
void Pregunta::setEnunciado(const std::string& enunc) {
    enunciado = enunc;
}

void Pregunta::setOpciones(const std::vector<std::string>& opts) {
    // Solo permite establecer opciones si el tipo es opcion Multiple
    if (tipo == TipoPregunta::OPCION_MULTIPLE) {
         if (!opts.empty()) {
             opciones = opts;
         } else {
              std::cerr << "Error: Intento de asignar opciones vacías a pregunta de Opción Múltiple (ID " << id << ")." << std::endl;
         }
    } else {
        std::cerr << "Error: No se pueden asignar opciones a una pregunta que no es de Opción Múltiple (ID " << id << ")." << std::endl;
    }
}

void Pregunta::setRespuestaCorrecta(const std::string& resp) {
    respuestaCorrecta = resp;
}

void Pregunta::setNivelTaxonomico(NivelBloom niv) {
    if (niv != NivelBloom::INVALIDO) {
        nivelTaxonomico = niv;
    } else {
         std::cerr << "Advertencia: Intento de asignar nivel taxonómico inválido (ID " << id << ")." << std::endl;
    }
}

void Pregunta::setTiempoEstimado(int tiempo) {
    if (tiempo > 0) {
        tiempoEstimado = tiempo;
    } else {
        std::cerr << "Advertencia: El tiempo estimado debe ser positivo. No se actualizó (ID " << id << ")." << std::endl;
    }
}

void Pregunta::setAnioUltimoUso(int anio) {
    anioUltimoUso = anio;
}

// Metodo para mostrar la informacion de la pregunta (formato para el usuario)
void Pregunta::mostrar() const {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Tipo: " << tipoPreguntaToString(tipo) << std::endl;
    std::cout << "Nivel Bloom: " << nivelBloomToString(nivelTaxonomico) << std::endl;
    std::cout << "Tiempo Estimado: " << tiempoEstimado << " min" << std::endl;
    std::cout << "Enunciado: " << enunciado << std::endl;

    if (tipo == TipoPregunta::OPCION_MULTIPLE) {
        std::cout << "Opciones:" << std::endl;
        char letraOpcion = 'A';
        for (const auto& opcion : opciones) {
            std::cout << "  " << letraOpcion++ << ") " << opcion << std::endl;
        }
    }
     // Mostrar el año de ultimo uso para el usuario
     if(anioUltimoUso > 0) {
         std::cout << "Ultimo Uso: " << anioUltimoUso << std::endl;
     }
     std::cout << "----------------------------------------" << std::endl;
}
