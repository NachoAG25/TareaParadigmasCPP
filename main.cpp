#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <ios>
#include <algorithm> // Para std::find_if, std::shuffle, std::transform
#include <cctype>    // Para ::tolower
#include <chrono>    // Para obtener el año actual
#include <ctime>     // Para obtener el año actual
#include <random>    // Para std::shuffle, std::random_device, std::mt19937
#include "pregunta.h"

// Declaraciones de Funciones
void crearPregunta(std::vector<Pregunta>& banco);
void actualizarPregunta(std::vector<Pregunta>& banco);
void borrarPregunta(std::vector<Pregunta>& banco);
void consultarPregunta(const std::vector<Pregunta>& banco);
void buscarPreguntaPorTaxonomia(const std::vector<Pregunta>& banco);
void generarEvaluacion(std::vector<Pregunta>& banco);

// Funciones Auxiliares

// Muestra el menu principal al usuario
void mostrarMenu() {
    std::cout << "\n===== MENU PRINCIPAL =====" << std::endl;
    std::cout << "1. Crear nueva pregunta" << std::endl;
    std::cout << "2. Actualizar pregunta existente" << std::endl;
    std::cout << "3. Borrar pregunta existente" << std::endl;
    std::cout << "4. Consultar pregunta por ID" << std::endl;
    std::cout << "5. Buscar preguntas por Nivel Bloom" << std::endl;
    std::cout << "6. Generar Evaluacion" << std::endl;
    std::cout << "7. Salir" << std::endl;
    std::cout << "==========================" << std::endl;
}

// Funcion para obtener una opcion numerica valida del menu
int obtenerOpcionValida(int minOpcion, int maxOpcion) {
    int opcion;
    while (true) {
         mostrarMenu(); // Muestra el menu antes de pedir la opcion
        std::cout << "Ingrese su opcion (" << minOpcion << "-" << maxOpcion << "): ";
        if (!(std::cin >> opcion)) { // Si no es numero
            std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpia buffer
            if (opcion >= minOpcion && opcion <= maxOpcion) {
                return opcion; // Opcion valida
            } else {
                std::cout << "Opcion fuera de rango. Intente de nuevo." << std::endl;
            }
        }
         if(opcion < minOpcion || opcion > maxOpcion){
             std::cout << "\nPresione Enter para reintentar...";
             std::cin.get(); // Espera Enter
         }
    }
}

// Funcion auxiliar para leer enteros positivos
int obtenerEnteroPositivo(const std::string& prompt) {
    int numero;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> numero)) {
            std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (numero > 0) {
                return numero; // Numero valido
            } else {
                std::cout << "El numero debe ser positivo. Intente de nuevo." << std::endl;
            }
        }
    }
}

// Funcion auxiliar para leer cualquier entero
int obtenerEntero(const std::string& prompt) {
    int numero;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> numero)) {
            std::cout << "Entrada invalida. Por favor ingrese un numero entero." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return numero; // Devuelve el número leído
        }
    }
}


// Funcion Principal (main) 
int main() {
    std::vector<Pregunta> bancoPreguntas; // Aqui se guardaran todas las preguntas
    int opcion;

    std::cout << "Bienvenido al Sistema de Gestion de Evaluaciones" << std::endl;

    do {
        // mostrarMenu(); Se llama dentro de obtenerOpcionValida
        opcion = obtenerOpcionValida(1, 7);

        switch (opcion) {
             case 1: // Crear
                std::cout << "\n--- Crear Pregunta ---" << std::endl;
                crearPregunta(bancoPreguntas);
                break;
            case 2: // Actualizar
                std::cout << "\n--- Actualizar Pregunta ---" << std::endl;
                actualizarPregunta(bancoPreguntas);
                 break;
            case 3: // Borrar
                 std::cout << "\n--- Borrar Pregunta ---" << std::endl;
                 borrarPregunta(bancoPreguntas);
                break;
            case 4: // Consultar
                 std::cout << "\n--- Consultar Pregunta por ID ---" << std::endl;
                 consultarPregunta(bancoPreguntas);
                break;
            case 5: // Buscar Bloom
                 std::cout << "\n--- Buscar Preguntas por Nivel Bloom ---" << std::endl;
                 buscarPreguntaPorTaxonomia(bancoPreguntas);
                break;
            case 6: // Generar Evaluacion
                 std::cout << "\n--- Generar Evaluacion ---" << std::endl;
                 generarEvaluacion(bancoPreguntas);
                break;
            case 7: // Salir
                std::cout << "Saliendo del programa..." << std::endl;
                break;
        }
        // Pausa antes de volver a mostrar el menu (excepto al salir)
        if (opcion != 7) {
             std::cout << "\nPresione Enter para continuar...";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Intenta limpiar por si acaso
        }

    } while (opcion != 7);

    return 0; // Indica que el programa termino correctamente
}

// Implementacion de Funciones

void crearPregunta(std::vector<Pregunta>& banco) {
    std::string enunciado;
    TipoPregunta tipo = TipoPregunta::INVALIDO;
    std::vector<std::string> opciones;
    std::string respuestaCorrecta;
    NivelBloom nivel = NivelBloom::INVALIDO;
    int tiempoEstimado = 0;
    int tipoInt;
    std::string inputStr;

    std::cout << "Ingrese el enunciado de la pregunta: ";
    std::getline(std::cin, enunciado);

    while (tipo == TipoPregunta::INVALIDO) {
        std::cout << "Seleccione el tipo de pregunta:" << std::endl;
        std::cout << "  1. Opcion Multiple" << std::endl;
        std::cout << "  2. Verdadero/Falso" << std::endl;
        tipoInt = obtenerOpcionValida(1, 2); // Usa la funcion auxiliar modificada
        if (tipoInt == 1) tipo = TipoPregunta::OPCION_MULTIPLE;
        else tipo = TipoPregunta::VERDADERO_FALSO;
    }

    if (tipo == TipoPregunta::OPCION_MULTIPLE) {
        int numOpciones = obtenerEnteroPositivo("Ingrese el numero de opciones (>1): ");
        while(numOpciones <= 1) {
             std::cout << "Debe haber al menos 2 opciones." << std::endl;
             numOpciones = obtenerEnteroPositivo("Ingrese el numero de opciones (>1): ");
        }
        opciones.clear();
        char letraOpcion = 'A';
        for (int i = 0; i < numOpciones; ++i) {
            std::cout << "Ingrese el texto para la opcion " << letraOpcion++ << ": ";
            std::getline(std::cin, inputStr);
            opciones.push_back(inputStr);
        }
    }

    bool respuestaValida = false;
    while (!respuestaValida) {
        if (tipo == TipoPregunta::OPCION_MULTIPLE) {
            std::cout << "Ingrese la letra de la opcion correcta (A-" << char('A' + opciones.size() - 1) << "): ";
            std::getline(std::cin, respuestaCorrecta);
            if (respuestaCorrecta.length() == 1) {
                 char respChar = std::toupper(respuestaCorrecta[0]);
                 if (respChar >= 'A' && respChar < ('A' + opciones.size())) {
                      respuestaCorrecta = std::string(1, respChar);
                      respuestaValida = true;
                 }
            }
        } else { // Tipo VERDADERO_FALSO
            std::cout << "Ingrese la respuesta correcta (Verdadero/Falso): ";
            std::getline(std::cin, respuestaCorrecta);
            std::string lower_resp = respuestaCorrecta;
            std::transform(lower_resp.begin(), lower_resp.end(), lower_resp.begin(), ::tolower);
            if (lower_resp == "verdadero" || lower_resp == "falso") {
                 respuestaCorrecta = (lower_resp == "verdadero" ? "Verdadero" : "Falso");
                 respuestaValida = true;
            }
        }
        if (!respuestaValida) std::cout << "Respuesta invalida. Intente de nuevo." << std::endl;
    }

    while (nivel == NivelBloom::INVALIDO) {
        std::cout << "Ingrese el nivel de la taxonomia de Bloom (Recordar, Entender, Aplicar, Analizar, Evaluar, Crear): ";
        std::getline(std::cin, inputStr);
        nivel = stringToNivelBloom(inputStr);
        if (nivel == NivelBloom::INVALIDO) std::cout << "Nivel invalido. Intente de nuevo." << std::endl;
    }

    tiempoEstimado = obtenerEnteroPositivo("Ingrese el tiempo estimado en minutos: ");

    banco.emplace_back(enunciado, tipo, opciones, respuestaCorrecta, nivel, tiempoEstimado);

    std::cout << "\n[+] Pregunta ID " << banco.back().getId() << " creada exitosamente!" << std::endl;
}

void consultarPregunta(const std::vector<Pregunta>& banco) {
    if (banco.empty()) {
        std::cout << "El banco de preguntas esta vacio. No hay preguntas para consultar." << std::endl;
        return;
    }
    int idBuscado = obtenerEntero("Ingrese el ID de la pregunta a consultar: ");
    bool encontrada = false;
    for (const Pregunta& pregunta : banco) {
        if (pregunta.getId() == idBuscado) {
            std::cout << "\nInformacion de la Pregunta ID " << idBuscado << ":" << std::endl;
            pregunta.mostrar();
            encontrada = true;
            break;
        }
    }
    if (!encontrada) std::cout << "No se encontro ninguna pregunta con el ID " << idBuscado << "." << std::endl;
}

void buscarPreguntaPorTaxonomia(const std::vector<Pregunta>& banco) {
    if (banco.empty()) {
        std::cout << "El banco de preguntas esta vacio. No hay preguntas para buscar." << std::endl;
        return;
    }
    NivelBloom nivelBuscado = NivelBloom::INVALIDO;
    std::string inputStr;
    while (nivelBuscado == NivelBloom::INVALIDO) {
        std::cout << "Ingrese el nivel de Bloom a buscar (Recordar, Entender, Aplicar, Analizar, Evaluar, Crear): ";
        std::getline(std::cin, inputStr);
        nivelBuscado = stringToNivelBloom(inputStr);
        if (nivelBuscado == NivelBloom::INVALIDO) std::cout << "Nivel invalido ingresado. Por favor intente de nuevo." << std::endl;
    }
    std::cout << "\nBuscando preguntas con nivel Bloom: " << nivelBloomToString(nivelBuscado) << "..." << std::endl;
    int contador = 0;
    for (const Pregunta& pregunta : banco) {
        if (pregunta.getNivelTaxonomico() == nivelBuscado) {
            pregunta.mostrar();
            contador++;
        }
    }
    if (contador == 0) std::cout << "No se encontraron preguntas para el nivel " << nivelBloomToString(nivelBuscado) << "." << std::endl;
    else std::cout << "\nSe encontraron " << contador << " pregunta(s) para el nivel " << nivelBloomToString(nivelBuscado) << "." << std::endl;
}

void borrarPregunta(std::vector<Pregunta>& banco) {
    if (banco.empty()) {
        std::cout << "El banco de preguntas esta vacio. No hay preguntas para borrar." << std::endl;
        return;
    }
    int idBorrar = obtenerEntero("Ingrese el ID de la pregunta a borrar: ");
    auto it = std::find_if(banco.begin(), banco.end(), [idBorrar](const Pregunta& p) { return p.getId() == idBorrar; });
    if (it != banco.end()) {
        std::cout << "\nSe encontro la siguiente pregunta:" << std::endl;
        it->mostrar();
        char confirmacion = ' ';
        while (confirmacion != 's' && confirmacion != 'n') {
             std::cout << "\nEsta seguro que desea borrar esta pregunta? (s/n): ";
             std::cin >> confirmacion;
             confirmacion = std::tolower(confirmacion);
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             if(confirmacion != 's' && confirmacion != 'n') std::cout << "Por favor, ingrese 's' para si o 'n' para no." << std::endl;
        }
        if (confirmacion == 's') {
            banco.erase(it);
            std::cout << "Pregunta ID " << idBorrar << " borrada exitosamente." << std::endl;
        } else {
            std::cout << "Borrado cancelado por el usuario." << std::endl;
        }
    } else {
        std::cout << "No se encontro ninguna pregunta con el ID " << idBorrar << "." << std::endl;
    }
}

void actualizarPregunta(std::vector<Pregunta>& banco) {
     if (banco.empty()) {
        std::cout << "El banco de preguntas esta vacio. No hay preguntas para actualizar." << std::endl;
        return;
    }
    int idActualizar = obtenerEntero("Ingrese el ID de la pregunta a actualizar: ");
    auto it = std::find_if(banco.begin(), banco.end(), [idActualizar](const Pregunta& p) { return p.getId() == idActualizar; });
    if (it != banco.end()) {
        std::cout << "\nDatos actuales de la pregunta ID " << idActualizar << ":" << std::endl;
        it->mostrar();
        int opcionSubMenu;
        bool modificado = false;
        do {
            std::cout << "\nQue desea actualizar?" << std::endl;
            std::cout << "  1. Enunciado" << std::endl;
            if (it->getTipo() == TipoPregunta::OPCION_MULTIPLE) {
                 std::cout << "  2. Opciones (requiere reingresar respuesta correcta)" << std::endl;
                 std::cout << "  3. Respuesta Correcta (Letra)" << std::endl;
            } else {
                 std::cout << "  2. --- (No aplicable para V/F)" << std::endl;
                 std::cout << "  3. Respuesta Correcta (Verdadero/Falso)" << std::endl;
            }
            std::cout << "  4. Nivel Bloom" << std::endl;
            std::cout << "  5. Tiempo Estimado" << std::endl;
            std::cout << "  6. Cancelar y volver al menu principal" << std::endl;
            opcionSubMenu = obtenerOpcionValida(1, 6); // Reutiliza la validación, pide opción del 1 al 6
            std::string nuevoTexto;
            std::vector<std::string> nuevasOpciones;
            std::string nuevaRespuesta;
            NivelBloom nuevoNivel;
            int nuevoTiempo;
            switch (opcionSubMenu) {
                case 1: // Enunciado
                    std::cout << "Ingrese el nuevo enunciado: ";
                    std::getline(std::cin, nuevoTexto);
                    it->setEnunciado(nuevoTexto);
                    modificado = true;
                    std::cout << "[OK] Enunciado actualizado." << std::endl;
                    break;
                case 2: // Opciones
                    if (it->getTipo() == TipoPregunta::OPCION_MULTIPLE) {
                        int numOpciones = obtenerEnteroPositivo("Ingrese el NUEVO numero de opciones (>1): ");
                         while(numOpciones <= 1) {
                             std::cout << "Debe haber al menos 2 opciones." << std::endl;
                             numOpciones = obtenerEnteroPositivo("Ingrese el NUEVO numero de opciones (>1): ");
                         }
                        nuevasOpciones.clear();
                        char letraOpcion = 'A';
                        for (int i = 0; i < numOpciones; ++i) {
                            std::cout << "Ingrese texto para NUEVA opcion " << letraOpcion++ << ": ";
                            std::getline(std::cin, nuevoTexto);
                            nuevasOpciones.push_back(nuevoTexto);
                        }
                        it->setOpciones(nuevasOpciones);
                        std::cout << "ATENCION: Las opciones cambiaron, debe reingresar la respuesta correcta." << std::endl;
                        bool respuestaValida = false;
                        while(!respuestaValida) {
                            std::cout << "Ingrese la letra de la NUEVA opcion correcta (A-" << char('A' + nuevasOpciones.size() - 1) << "): ";
                            std::getline(std::cin, nuevaRespuesta);
                            if (nuevaRespuesta.length() == 1) {
                                char respChar = std::toupper(nuevaRespuesta[0]);
                                if (respChar >= 'A' && respChar < ('A' + nuevasOpciones.size())) {
                                    nuevaRespuesta = std::string(1, respChar);
                                    it->setRespuestaCorrecta(nuevaRespuesta);
                                    respuestaValida = true;
                                }
                            }
                             if (!respuestaValida) std::cout << "Respuesta invalida. Intente de nuevo." << std::endl;
                        }
                        modificado = true;
                        std::cout << "[OK] Opciones y respuesta correcta actualizadas." << std::endl;
                    } else {
                        std::cout << "Opcion no valida para preguntas de Verdadero/Falso." << std::endl;
                    }
                    break;
                case 3: // Respuesta Correcta
                    {
                        bool respuestaValida = false;
                        while(!respuestaValida) {
                             if (it->getTipo() == TipoPregunta::OPCION_MULTIPLE) {
                                 std::cout << "Ingrese la NUEVA letra correcta (A-" << char('A' + it->getOpciones().size() - 1) << "): ";
                                 std::getline(std::cin, nuevaRespuesta);
                                 if (nuevaRespuesta.length() == 1) {
                                     char respChar = std::toupper(nuevaRespuesta[0]);
                                     if (respChar >= 'A' && respChar < ('A' + it->getOpciones().size())) {
                                         nuevaRespuesta = std::string(1, respChar);
                                         it->setRespuestaCorrecta(nuevaRespuesta);
                                         respuestaValida = true;
                                     }
                                 }
                             } else { // VERDADERO_FALSO
                                 std::cout << "Ingrese la NUEVA respuesta correcta (Verdadero/Falso): ";
                                 std::getline(std::cin, nuevaRespuesta);
                                 std::string lower_resp = nuevaRespuesta;
                                 std::transform(lower_resp.begin(), lower_resp.end(), lower_resp.begin(), ::tolower);
                                 if (lower_resp == "verdadero" || lower_resp == "falso") {
                                     nuevaRespuesta = (lower_resp == "verdadero" ? "Verdadero" : "Falso");
                                     it->setRespuestaCorrecta(nuevaRespuesta);
                                     respuestaValida = true;
                                 }
                             }
                             if (!respuestaValida) std::cout << "Respuesta invalida. Intente de nuevo." << std::endl;
                        }
                         modificado = true;
                         std::cout << "[OK] Respuesta correcta actualizada." << std::endl;
                    }
                    break;
                case 4: // Nivel Bloom
                    nuevoNivel = NivelBloom::INVALIDO;
                    while (nuevoNivel == NivelBloom::INVALIDO) {
                        std::cout << "Ingrese el NUEVO nivel Bloom (Recordar, Entender, Aplicar, Analizar, Evaluar, Crear): ";
                        std::getline(std::cin, nuevoTexto);
                        nuevoNivel = stringToNivelBloom(nuevoTexto);
                        if (nuevoNivel == NivelBloom::INVALIDO) {
                            std::cout << "Nivel invalido. Intente de nuevo." << std::endl;
                        } else {
                            it->setNivelTaxonomico(nuevoNivel);
                            modificado = true;
                             std::cout << "[OK] Nivel Bloom actualizado." << std::endl;
                        }
                    }
                    break;
                case 5: // Tiempo Estimado
                    nuevoTiempo = obtenerEnteroPositivo("Ingrese el NUEVO tiempo estimado en minutos: ");
                    it->setTiempoEstimado(nuevoTiempo);
                    modificado = true;
                     std::cout << "[OK] Tiempo estimado actualizado." << std::endl;
                    break;
                case 6: // Cancelar
                    std::cout << "Volviendo al menu principal..." << std::endl;
                    break;
                default:
                    std::cout << "Opcion de sub-menu invalida." << std::endl;
                    break;
            }
             // Pausa breve dentro del submenu si no es cancelar
             if (opcionSubMenu != 6) {
                  std::cout << "\nPresione Enter para continuar actualizando...";
                  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Intenta limpiar
             }
        } while (opcionSubMenu != 6);
        if (modificado) {
             std::cout << "\nDatos finales de la pregunta ID " << idActualizar << ":" << std::endl;
             it->mostrar();
        }
    } else {
        std::cout << "No se encontro ninguna pregunta con el ID " << idActualizar << "." << std::endl;
    }
}

void generarEvaluacion(std::vector<Pregunta>& banco) {
    if (banco.empty()) {
        std::cout << "El banco de preguntas esta vacio. No se puede generar una evaluacion." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    #pragma warning(suppress : 4996) // Suprimir advertencia de localtime en MSVC si es necesario
    auto tm_local = *std::localtime(&time_t_now);
    int anioActual = tm_local.tm_year + 1900;

    int numPreguntasTotal = obtenerEnteroPositivo("Ingrese el numero total de preguntas para la evaluacion: ");

    std::vector<Pregunta> evaluacionGenerada;
    std::vector<int> idsUsadosEnEstaEvaluacion;
    int tiempoTotalEstimado = 0;
    bool exito = true;

    for (int i = 0; i < numPreguntasTotal; ++i) {
        std::cout << "\n--- Pregunta #" << (i + 1) << " ---" << std::endl;

        NivelBloom nivelDeseado = NivelBloom::INVALIDO;
        std::string inputStr;
        while (nivelDeseado == NivelBloom::INVALIDO) {
            std::cout << "Ingrese el nivel Bloom deseado para esta pregunta (Recordar, Entender, Aplicar, Analizar, Evaluar, Crear): ";
            std::getline(std::cin, inputStr);
            nivelDeseado = stringToNivelBloom(inputStr);
            if (nivelDeseado == NivelBloom::INVALIDO) std::cout << "Nivel invalido. Intente de nuevo." << std::endl;
        }

        std::vector<int> indicesCandidatas;
        for (int idx = 0; idx < banco.size(); ++idx) {
            const Pregunta& p = banco[idx];
            if (p.getNivelTaxonomico() != nivelDeseado) continue;
            if (p.getAnioUltimoUso() > 0 && p.getAnioUltimoUso() >= (anioActual - 1)) continue;
            bool usadaYa = false;
            for (int idUsado : idsUsadosEnEstaEvaluacion) { if (p.getId() == idUsado) { usadaYa = true; break; } }
            if (usadaYa) continue;
            indicesCandidatas.push_back(idx);
        }

        if (indicesCandidatas.empty()) {
            std::cout << "\n[!] Error: No se encontraron preguntas validas y disponibles para el nivel " << nivelBloomToString(nivelDeseado) << " que cumplan las restricciones." << std::endl;
            std::cout << "Generacion de evaluacion abortada." << std::endl;
            exito = false;
            break;
        } else {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(indicesCandidatas.begin(), indicesCandidatas.end(), g);
            int indiceSeleccionado = indicesCandidatas[0];
            evaluacionGenerada.push_back(banco[indiceSeleccionado]);
            idsUsadosEnEstaEvaluacion.push_back(banco[indiceSeleccionado].getId());
            tiempoTotalEstimado += banco[indiceSeleccionado].getTiempoEstimado();
            banco[indiceSeleccionado].setAnioUltimoUso(anioActual);
             std::cout << "[OK] Pregunta ID " << banco[indiceSeleccionado].getId() << " seleccionada para el nivel " << nivelBloomToString(nivelDeseado) << "." << std::endl;
        }
    }

    if (exito) {
        std::cout << "\n\n===== EVALUACION GENERADA =====" << std::endl;
        if(evaluacionGenerada.empty()) {
             std::cout << "(No se seleccionó ninguna pregunta)" << std::endl;
        } else {
            for(const auto& preg : evaluacionGenerada) {
                preg.mostrar();
            }
            std::cout << "==============================" << std::endl;
            std::cout << "TIEMPO TOTAL ESTIMADO: " << tiempoTotalEstimado << " minutos." << std::endl;
            std::cout << "==============================" << std::endl;
        }
    }
}
