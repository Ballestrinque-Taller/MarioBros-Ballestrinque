#include "LectorXML.h"
#include "Log.h"
#include <time.h>

#define CAMPO_ENEMIGOS "enemigos"
#define CAMPO_BLOQUES "bloques"
#define CAMPO_MONEDAS "monedas"
#define CAMPO_BACKGROUND "background"
#define CAMPO_TIMER "tiempo"
#define WIDTH 800
#define ALTO_VENTANA 600
#define RANGO_MONEDAS 200
#define POS_MIN_MONEDAS 275

using namespace rapidxml;

LectorXML::LectorXML(std::string path_to_xml){
    SET_LOGGING_LEVEL(Log::ERROR); //No puede ser mas baja que error por defecto.
    srand(time(NULL));
    if (!std::ifstream(path_to_xml)){
        LOG(Log::ERROR)<<"No se encontro el archivo xml del siguiente path: "<<path_to_xml<<std::endl;
        set_default();
        return;
    }
    rapidxml::file<> xmlFile(path_to_xml.c_str());
    archivo_data = xmlFile.data();
    documento.parse<0>((char*)archivo_data.c_str());
    xml_attribute<>* cantidad_jugadores = documento.first_node()->first_node("jugadores")->first_attribute("cantidad");
    if (!cantidad_jugadores){
        LOG(Log::ERROR)<< "No esta establecida la cantidad  de jugadores. Se settea en 2."<<std::endl;
        cant_jugadores = 2;
    }
    else {
        cant_jugadores = std::stoi(cantidad_jugadores->value());
    }
}

bool chequear_atributos_enemigo(xml_node<>* enemigo){
    if (enemigo->first_attribute("tipo") == nullptr){
        return false;
    }
    if (enemigo->first_attribute("imagen") == nullptr){
        return false;
    }
    if (enemigo->first_attribute("cantidad") == nullptr){
        return false;
    }
    return true;
}

//genera todos los enemigos del nivel.
bool LectorXML::generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*>* enemigos){
    xml_node<>* nodo_de_enemigos = nivel->first_node(CAMPO_ENEMIGOS);
    if (nodo_de_enemigos == nullptr) {
        LOG(Log::ERROR) << "No se encontro la seccion de enemigos en el XML." << std::endl;
        return false;
    }

    //dentro del campo enemigos en el xml, recorro cada enemigo (goomba, troopa, etc)
    xml_node<>* enemigo = nodo_de_enemigos->first_node();
    if(enemigo == nullptr) {
        LOG(Log::ERROR) << "No se ha encontrado a ningun enemigo en el XML. Seccion Enemigos." << std::endl;
        return false;
    }

   while(enemigo != nullptr){
        LOG(Log::INFO)<<"Generando enemigo... "<<std::endl;

        if (!chequear_atributos_enemigo(enemigo)){
            LOG(Log::ERROR)<<"Fallo en los campos de atributos de enemigo."<<std::endl;
            return false;
        }

        std::string tipo_enemigo = enemigo->first_attribute("tipo")->value();
        LOG(Log::DEBUG)<<"Tipo de enemigo: "<< tipo_enemigo << std::endl;

        std::string path_a_imagen = (enemigo->first_attribute("imagen")->value());
        LOG(Log::DEBUG)<<"Imagen a cargar: "<< path_a_imagen << std::endl;

        int cantidad = std::stoi(enemigo->first_attribute("cantidad")->value());
        LOG(Log::DEBUG)<<"Cantidad a generar: "<< cantidad << std::endl;
        if (cantidad < 0){
            LOG(Log::ERROR)<<"Cantidad invalida. No puede ser negativa, no se generaran enemigos de tipo "<<tipo_enemigo<<"."<<std::endl;
            cantidad = 0;
        }

        generar_enemigos_particulares(tipo_enemigo,path_a_imagen,cantidad,enemigos);
        enemigo = enemigo->next_sibling();
   }
    return true;
}

//genera todos los enemigos de UN SOLO tipo en particular. Ejemplo: genera TODOS los troopas.
void LectorXML::generar_enemigos_particulares(std::string tipo_enemigo, std::string path_to_image, int cantidad, std::vector<Enemigo*>* enemigos){
    if(tipo_enemigo.compare("troopa") == 0){
        for(int i=0;i<cantidad;i++){
            Enemigo* enemigo = new Tortuga(rand() % (ancho_ajustado-WIDTH)+WIDTH ,0,path_to_image);
            enemigos->push_back(enemigo);
        }
    }
    else if(tipo_enemigo.compare("goomba") == 0){
        for(int i=0;i<cantidad;i++){
            Enemigo* enemigo = new Goomba(rand() % (ancho_ajustado-WIDTH)+WIDTH ,0,path_to_image);
            enemigos->push_back(enemigo);
        }
    }
}

bool chequear_atributos_bloque(xml_node<>* bloque){
    if (bloque->first_attribute("x") == nullptr){
        return false;
    }
    if (bloque->first_attribute("y") == nullptr){
        return false;
    }
    if (bloque->first_attribute("cantidad") == nullptr){
        return false;
    }
    if (bloque->first_attribute("tipo") == nullptr){
        return false;
    }
    if (bloque->first_attribute("imagen") == nullptr){
        return false;
    }
    return true;
}

bool LectorXML::generar_escenario(std::vector<Escenario*>* escenarios, xml_node<>* nivel){
    xml_node<>* nodo_de_bloques = nivel->first_node(CAMPO_BLOQUES);
    if (nodo_de_bloques != nullptr) {
        LOG(Log::INFO)<<"Leyendo los bloques del nivel." << std::endl;
        LOG(Log::DEBUG) << "Valor del puntero a bloques: " << nodo_de_bloques << std::endl;
    } else {
        LOG(Log::ERROR) << "No se encontraron los bloques del nivel. Valor del puntero: " << nodo_de_bloques << std::endl;
        return false;
    }
    xml_node<>* bloque = nodo_de_bloques->first_node();

    while(bloque != nullptr){

        LOG(Log::INFO)<<"Leyendo bloque..."<<std::endl;

        if(!chequear_atributos_bloque(bloque)){
            return false;
        }

        std::string tipo = bloque->first_attribute("tipo")->value();
        LOG(Log::DEBUG)<<"Tipo de bloque : "<<tipo<<std::endl;
        int cantidad = std::stoi(bloque->first_attribute("cantidad")->value());
        LOG(Log::DEBUG)<<"Cargando cantidad de bloques consecutivos. Cantidad: "<<cantidad<<std::endl;
        if (cantidad < 0){
            LOG(Log::ERROR)<<"La cantidad de bloques indicada es negativa, y no es valida. Se generaran 0 bloques de tipo "<<tipo<<std::endl;
            cantidad = 0;
        }

        int pos_x = std::stoi(bloque->first_attribute("x")->value());
        LOG(Log::DEBUG)<<"Cargando posicion en x. Valor: "<<pos_x<<std::endl;
        if (pos_x < 0){
            LOG(Log::ERROR)<<"Posicion en x invalida para el bloque, generandolo en la posicion x=0"<<std::endl;
            pos_x = 0;
        }
        else if (pos_x > ancho_ajustado){
            LOG(Log::ERROR)<<"Posicion invalida en x, supera el limite del nivel. Seteandolo en el final del nivel."<<std::endl;
            pos_x = ancho_ajustado - 16*cantidad;
        }

        int pos_y = std::stoi(bloque->first_attribute("y")->value());
        LOG(Log::DEBUG)<<"Cargando posicion en y. Valor: "<<pos_y<<std::endl;
        if (pos_y < 0){
            LOG(Log::ERROR)<<"Posicion en y invalida para el bloque, generandolo en la posicion y=0"<<std::endl;
            pos_y = 0;
        }
        else if (pos_y > ALTO_VENTANA){
            LOG(Log::ERROR)<<"Posicion en y invalida, supera el limite del nivel. Seteandolo en el limite inferior de la ventana."<<std::endl;
            pos_y = ALTO_VENTANA - 16;
        }

        std::string path = bloque->first_attribute("imagen")->value();
        LOG(Log::DEBUG)<<"Path al bloque a renderizar: "<<path<<std::endl;

        generar_bloques_particulares(tipo, cantidad, pos_x, pos_y, path, escenarios);
        bloque = bloque->next_sibling();
    }
    return true;
}

void LectorXML::generar_bloques_particulares(std::string tipo, int cantidad, int x, int y, std::string path, std::vector<Escenario*>* escenarios){
    if (tipo.compare("Ladrillo") == 0){
        for (int i=0; i<cantidad; i++){
            escenarios->push_back(new Ladrillo(x+i*ANCHO_LADRILLO_PANTALLA,y, path));
        }
    }
    else if(tipo.compare("Sorpresa") == 0){
        for (int i=0; i<cantidad; i++){
            escenarios->push_back(new Sorpresa(x+i*ANCHO_SORPRESA_PANTALLA,y, path));
        }
    }else if(tipo.compare("Tuberia") == 0){
        std::string png_str(".png");
        for (int i = 0; i < png_str.size(); i++)
            path.erase(path.size() - 1);
        std::string path_sup = path + "_TAPA.png";
        std::string path_inf = path + "_CUERPO.png";
        escenarios->push_back(new Tuberia(x,y - cantidad * ALTO_TUBERIA_PANTALLA, path_sup));
        for (int i=cantidad-1; i>=0; i--){
            escenarios->push_back(new Tuberia(x,y-i*ALTO_TUBERIA_PANTALLA, path_inf));
        }
    }

}

bool colision_moneda(Renderer* entidad_almacenada, Renderer* entidad_a_colisionar){
    SDL_Rect entidad_almacenada_rect = entidad_almacenada->get_dest_rect();
    SDL_Rect entidad_col_rect = entidad_a_colisionar->get_dest_rect();
    if(((entidad_almacenada_rect.x <= (entidad_col_rect.x + entidad_col_rect.w)) && ((entidad_almacenada_rect.x + entidad_almacenada_rect.w) >= entidad_col_rect.x))
        && ((entidad_almacenada_rect.y <= (entidad_col_rect.y + entidad_col_rect.h)) && ((entidad_almacenada_rect.y + entidad_almacenada_rect.h) >= entidad_col_rect.y))){
        return true;
    }
    return false;
}

bool LectorXML::moneda_colisiona_con_algun_bloque(int pos_x_moneda, int pos_y_moneda, std::vector<Escenario*> escenarios){
    bool colisiona = false;
    Moneda* moneda = new Moneda(pos_x_moneda, pos_y_moneda,"");
    for(auto& escenario: escenarios){
        if(colision_moneda(moneda, escenario)){
            colisiona = true;
            break;
        }
    }
    return colisiona;
}

bool LectorXML::generar_monedas(xml_node<>* nivel, std::vector<Moneda*>* monedas, std::vector<Escenario*>escenarios){
    xml_node<>* nodo_de_monedas = nivel->first_node(CAMPO_MONEDAS);
    if(nodo_de_monedas != nullptr) {
        LOG(Log::INFO) << "Monedas leidas correctamente del archivo XML." << std::endl;
    }else{
        LOG(Log::ERROR) << "Seccion de monedas no encontrada." << std::endl;
        return false;
    }
    if (nodo_de_monedas->first_attribute("cantidad") == nullptr){
        LOG(Log::ERROR) << "Error en el campo monedas. No existe cantidad" << std::endl;
        return false;
    }
    int cantidad = std::stoi(nodo_de_monedas->first_attribute("cantidad")->value());
    if (cantidad < 0){
        LOG(Log::ERROR) << "No puede haber menos de 0 monedas. Setteando cantidad a 0." << std::endl;
        cantidad = 0;
    }
    LOG(Log::DEBUG) << "Cantidad de monedas a generar: " << cantidad << std::endl;
    std::string path = nodo_de_monedas->first_attribute("imagen")->value();
    for (int i=0; i<cantidad; i++){
        int pos_x, pos_y;
        do {
            pos_x = rand() % ancho_ajustado;
            pos_y = rand() % RANGO_MONEDAS + POS_MIN_MONEDAS;
        }while(moneda_colisiona_con_algun_bloque(pos_x, pos_y, escenarios));
        monedas->push_back(new Moneda(pos_x,pos_y, path));
    }
    return true;
}

bool chequear_campos_background(xml_node<>* nodo_de_background){
    if (nodo_de_background->first_attribute("imagen") == nullptr){
        return false;
    }
    if (nodo_de_background->first_attribute("ancho") == nullptr){
        return false;
    }
    if (nodo_de_background->first_attribute("alto") == nullptr){
        return false;
    }
    return true;
}

bool LectorXML::generar_background(xml_node<>* nivel, Background** background){
    xml_node<>* nodo_de_background = nivel->first_node(CAMPO_BACKGROUND);
    if(nodo_de_background) {
        LOG(Log::INFO) << "Background leido correctamente del archivo XML." << std::endl;
    }
    else {
        LOG(Log::ERROR) << "Background leido incorrectamente del archivo XML. Archivo no encontrado" << std::endl;
        return false;
    }
    std::string path = nodo_de_background->first_attribute("imagen")->value();
    LOG(Log::DEBUG)<<"Path a background leido. Valor: "<<path<<std::endl;
    int ancho = std::stoi (nodo_de_background->first_attribute("ancho")->value());
    LOG(Log::DEBUG)<<"Ancho de background leido. Valor: "<<ancho<<std::endl;
    if (ancho <= 0){
        LOG(Log::ERROR)<<"Ancho de background invalido. No puede ser menor o igual a 0."<<std::endl;
        return false;
    }
    int alto = std::stoi (nodo_de_background->first_attribute("alto")->value());
    if (alto <= 0){
        LOG(Log::ERROR)<<"Alto de background invalido. No puede ser menor o igual a 0."<<std::endl;
        return false;
    }
    LOG(Log::DEBUG)<<"Alto de background leido. Valor: "<<alto<<std::endl;
    ancho_ajustado = ancho*WIDTH/(alto*RATIO_ASPECTO);
    (*background) = new Background(path, ancho, alto);
    return true;
}

bool LectorXML::generar_timer(xml_node<>* nivel, Temporizador** timer){
    xml_node<>* nodo_de_timer = nivel->first_node(CAMPO_TIMER);
    if (nodo_de_timer == nullptr){
        LOG(Log::ERROR)<<"Temporizador no encontrado en el XML."<<std::endl;
        return false;
    }
    if (nodo_de_timer->first_attribute("cantidad") == nullptr){
        LOG(Log::ERROR)<<"Cantidad del timer no encontrado."<<std::endl;
        return false;
    }
    int cantidad = std::stoi(nodo_de_timer->first_attribute("cantidad")->value());
    LOG(Log::DEBUG)<<"Tiempo de timer leido. Valor: "<<cantidad<<std::endl;
    if (cantidad <= 0){
        LOG(Log::ERROR)<<"Tiempo invalido por ser menor o igual a 0. Seteando al tiempo default: 500"<<std::endl;
        cantidad = 500;
    }
    if((*timer) == nullptr) {
        LOG(Log::DEBUG) << "Creando e inicializando primer temporizador." << std::endl;
        (*timer) = new Temporizador(cantidad);
    }
    else{
        LOG(Log::DEBUG) << "Reiniciando temporizador..." << std::endl;
        (*timer)->reiniciar((size_t) cantidad);
    }
    return true;
}

void set_log_level(std::string level){
    if(level.compare("INFO") == 0) {
        SET_LOGGING_LEVEL(Log::INFO);
    }
    else if (level.compare("ERROR") == 0) {
        SET_LOGGING_LEVEL(Log::ERROR);
    }
    else { //Caso por defecto. Carga debug que es el mas completo.
        SET_LOGGING_LEVEL(Log::DEBUG);
    }
}

int LectorXML::generar_nivel(std::vector<Enemigo*>* enemigos, std::vector<Moneda*>* monedas, std::vector<Escenario*>* escenarios, Background** background, Temporizador** temporizador, std::string nivel) {
    for (auto &enemigo : (*enemigos)) {
        delete enemigo;
    }
    for (auto &escenario : (*escenarios)) {
        delete escenario;
    }
    enemigos->clear();
    escenarios->clear();
    delete (*background);
    if (documento.first_node()->first_node("log") != nullptr) {
        set_log_level(documento.first_node()->first_node("log")->first_attribute()->value());
    }
    else{
        SET_LOGGING_LEVEL(Log::DEBUG);
        LOG(Log::ERROR)<<"El tipo de log no se encuentra especificado en el XML. Procediendo a modo DEBUG."<<std::endl;
    }
    xml_node<> *nodo_del_nivel = documento.first_node()->first_node(nivel.c_str());
    LOG(Log::INFO) << "Cargando " << nivel << "..." << std::endl;
    if (!nodo_del_nivel) {
        LOG(Log::INFO) << "Nivel no encontrado... Procediendo a finalizar el juego." << std::endl;
        return QUIT;
    }
    if (!generar_background(nodo_del_nivel, background)) {
        return ERROR_XML;
    }
    if (!generar_escenario(escenarios, nodo_del_nivel)) {
        return ERROR_XML;
    }
    if (!generar_enemigos(nodo_del_nivel, enemigos)) {
        return ERROR_XML;
    }
    if (!generar_monedas(nodo_del_nivel, monedas, (*escenarios))){
        return ERROR_XML;
    }
    if(!generar_timer(nodo_del_nivel, temporizador)) {
        return ERROR_XML;
    }
    return PASO;
}

bool LectorXML::generar_jugador(std::vector<Jugador*>* jugadores){
    xml_node<>* nodo_de_jugadores = documento.first_node()->first_node("jugadores");
    if (nodo_de_jugadores == nullptr){
         LOG(Log::ERROR) << "Jugadores no encontrados en el XML." << std::endl;
         return false;
    }
    else{
        LOG (Log::INFO) << "Cantidad de jugadores a generar: " << cant_jugadores << std::endl;

        for (int i=0; i<cant_jugadores; i++) {
            LOG(Log::INFO) << "Leyendo jugador #" << (jugadores->size() + 1) << std::endl;
            std::string jugador = std::string("jugador") + std::to_string((jugadores->size() + 1));
            std::string path_imagen = nodo_de_jugadores->first_node(jugador.c_str())->first_attribute()->value();
            jugadores->push_back(new Jugador(path_imagen));
        }
        return true;
    }
}

void LectorXML::set_default(){
    LOG(Log::ERROR)<<"Modificando al archivo por defecto..."<<std::endl;
    rapidxml::file<> xmlFile("./res/config.xml");
    archivo_data = xmlFile.data();
    documento.parse<0>((char*)archivo_data.c_str());
}

int LectorXML::get_cantidad_jugadores() {
    return cant_jugadores;
}

bool LectorXML::posee_credenciales(credenciales_t credenciales){
    bool posee = false;
    xml_node<>* nodo_de_credenciales = documento.first_node()->first_node("credenciales");
    xml_node<>* nodo_credencial = nodo_de_credenciales->first_node();
    while(nodo_credencial != nullptr && !posee){
        std::string usuario = nodo_credencial->first_attribute("usuario")->value();
        std::string password = nodo_credencial->first_attribute("password")->value();
        if (strcmp(credenciales.password,password.c_str())==0 && strcmp(credenciales.usuario,usuario.c_str())==0)
            posee = true;
        nodo_credencial = nodo_credencial->next_sibling();
    }
    return posee;
}

bool LectorXML::tiene_siguiente_nivel(std::string nivel){
    return(documento.first_node()->first_node(nivel.c_str()) != nullptr);
}