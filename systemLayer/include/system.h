

#ifndef SYSTEM_H
#define SYSTEM_H
#include "systemLayer_exp.h"

#ifdef WIN32
	#include "windowsSystem.h"
#else 
	#include "linuxSystem.h"
#endif


namespace  systemLayer
{
	#ifdef USE_QT
		class SL_LIB System : public QObject
	#else
		class SL_LIB System
	#endif
	{
		
		#ifdef USE_QT
			Q_OBJECT
		#endif
	public:
	
	public:
		System();
		~System();
		
		#ifdef USE_QT
			cv::Mat getPrintScreen();
		#endif

		/** Devuelve la posición del ratón desde el TOP_LEFT de la ventana.
			- <code> getMousePos() </code> .
			\pre

			\remark

			\par Ejemplo:
				\code
					// lectura de la posición del raton
					cv::Point p = viulib::System::getMousePos ();
				\endcode
		*/
			static cv::Point getMousePos();

		/** Envía un evento de ratón en el punto p al sistema.
			- <code> sendMouseEvent(evento,punto) </code> .
			\pre
				<code> (e debe pertenecer al enum M_EVENT) </code>
				<code> (p debe estar dentro de las dimensiones de la pantalla) </code>

			\remark
				En windows se utiliza SendInput.

			\par Ejemplo:
				\code
					// envío del evento de click con el botón izquierdo en el punto (10,20)
					viulib::System::sendMouseEvent (LEFT_CLICK,POINT(10,20));
				\endcode
		*/
			static void sendMouseEvent(M_EVENT e, cv::Point p);
		
		/** Envía un evento de teclado junto con el código de tecla.
			- <code> sendKeyboardEvent(evento,tecla) </code> .
			\pre
				<code> (e debe pertenecer al enum K_EVENT) </code>
			\remark
				En windows se utiliza SendInput.

			\par Ejemplo:
				\code
					// envío del evento de tecla A pulsada 
					viulib::System::sendKeyboardEvent (KEY,'A');
				\endcode
		*/
		static void sendKeyboardEvent(K_EVENT e,BYTE  c=0xFF);
		
		/** Función que devuelve el identificador al proceso con nombre lpcszFileName.
			- <code> FindProcess(nombreProceso) </code> .
			\pre
				<code> (lpcszFileName debe existir) </code>
			\remark
				
			\par Ejemplo:
				\code
					// buscar proceso notepad 
					DWORD processID = viulib::System::sendKeyboardEvent ("notepad.exe");
				\endcode
		*/
		static DWORD FindProcess( LPCTSTR lpcszFileName);
		
		/** Devuelve el número de Ticks del procesador.
			- <code> getTicks() </code> .
			\pre
			\remark
				
			\par Ejemplo:
				\code
					// buscar proceso notepad 
					unsigned long ticks = viulib::System::getTicks();
				\endcode
		*/
		static unsigned long getTicks();

		/** Devuelve el número de microsegundos n Alta Resolución (High Resolution) del procesador.
			- <code> getTicksHR() </code> .
			\pre
			\remark
				
			\par Ejemplo:
				\code
					// devolver los milisegundos entre dos lecturas del contador
					double t1 = viulib::System::getTicksHR();
					double t2 = viulib::System::getTicksHR();
					double time_ms = (t2-t1) * 0.001;
				\endcode
		*/
		static double getTicksHR();

		
		/** Redefine el area del escritorio
			- <code> resizeDesktopArea(Area) </code> .
			\pre
			\remark
				
			\par Ejemplo:
				\code
					// redefinir tamaño escritorio 
					unsigned long ticks = viulib::System::resizeDesktopArea(RECT(0,0,100,100));
				\endcode
		*/
		static void resizeDesktopArea(RECT rect);
		
		/** Obtiene una lista de las voces para el TTS disponibles en el sistema
			- <code> getVoiceList() </code> .
			\pre
				<code> requiere que se haya inicializado el TTS con initTTS </code>
				<code> requiere la definición de preproceso TTS_SUPPORT </code>
			\remark
				
			\par Ejemplo:
				\code
					// obtener la lista de voces
					viulib::System::initTTS();
					....
					std::vector<std::string> lista_voces= viulib::System::getVoiceList();
					if (lista_voces.isEmpty()) std::cout << "No hay voces instaladas."<<std::endl;
				\endcode
		*/
		static std::vector<std::string> getVoiceList();
		
		/** Cambia a una nueva voz el TTS
			- <code> changeVoice(identificador_voz) </code> .
			\pre
				<code> requiere que se haya inicializado el TTS con initTTS </code>
				<code> requiere la definición de preproceso TTS_SUPPORT </code>
			\remark
				
			\par Ejemplo:
				\code
					// cambiar a la voz llamada "Carmen"
					viulib::System::initTTS();
					....

					std::vector<std::string> lista_voces= viulib::System::getVoiceList();
					if (lista_voces.isEmpty()) std::cout << "No hay voces instaladas."<<std::endl;
					else
					{
						int id_voz = -1;
						for (int i=0;i<lista_voces.size();i++)
						{
							if (lista_voces.compare("Carmen")==0)
							{ 
								id_voz = i;
						 		break;
							}
						}
						if (id_voz!=-1) viulib::System::changeVoice(id_voz);
						else std::cout << "La voz Carmen no existe." << std::endl;
					}
				\endcode
		*/
		static int changeVoice(int i);
		
		/** Envia un texto para ser reproducido por la voz sintética
			- <code> speak(texto) </code> .
			\pre
				<code> requiere que se haya inicializado el TTS con initTTS </code>
				<code> requiere la definición de preproceso TTS_SUPPORT </code>
			\remark
				Soporta lenguaje etiquetado para cambios en el tono y en la velocidad del discurso
				SAPI TTS XML soporta cinco etiquetas que controlan el estado de la voz actual: Volume, Rate, Pitch, Emph, Spell, Pron, Silence, Voice, Lang, p, Partofsp, Context, Bookmark.
			\par Ejemplo:
				\code
					// Reproducir la frase "Dícenme que decís, exreina mía, que os dicen que yo he dicho aquel secreto. Y yo digo que os digo en un soneto que es decir por decir tal tontería."
					viulib::System::initTTS();
					....
					std::stringstream oss;
					std::string texto;
					oss << "<volume level=\"50\">";
					oss << "Dícenme que decís," << "<silence msec=\"500\"/>" << " exreina mía,";
					oss << "<volume level="100">";
					oss << "que os dicen que yo he dicho aquel" << "<emph> secreto </emph>!";
					oss << "</volume>"
					oss << "</volume>"
					oss << "<volume level=\"80\"/>";
					oss << "Y yo digo que os digo en un soneto que es decir por decir tal tontería."
					texto = oss.str();
					viulib::System::speak(texto);					
				\endcode
		*/
		static void speak(std::string texto);
		
		/** Inicializa el TTS del sistema
			- <code> initTTS() </code> .
			\pre
				<code> requiere la definición de preproceso TTS_SUPPORT </code>
			\remark
				
			\par Ejemplo:
				\code
					// inicializar tts
					viulib::System::initTTS();
				\endcode
		*/
		static void initTTS();
		
		/** Función que devuelve el tamaño de la pantalla
			- <code> getScreenSize(filas, columnas) </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// obtener el tamaño de la pantalla
					int filas, columnas;
					viulib::System::getScreenSize(filas, columnas);
				\endcode
		*/
		static void getScreenSize(int &rows, int &cols);


		/** Función que devuelve una marca de tiempo con el formato especificado
			- <code> time_stamp(formato) </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// requerir una marca de tiempo con formato Año-Mes-Día Hora_Minutos_Segundos
					std::string marca = viulib::System::time_stamp("%Y-%m-%d %H_%M_%S");
				\endcode
		*/
		static std::string time_stamp(std::string format);

		/** Devuelve una lista de los ficheros ubicados en el directorio indicado
			- <code> getdir(directorio,vector_de_ficheros) </code> .
			\pre
				
			\remark
				realiza la lista de manera recursiva accediendo también a los subdirectorios
				
			\par Ejemplo:
				\code
					// lista de ficheros del directorio "C:/temp"
					std::vector <string> lista_ficheros;
					viulib::System::getdir("C:/temp",lista_ficheros);
				\endcode
		*/
		static int getdir (std::string dir, std::vector<std::string> &files,std::string filter=std::string());
		
		/** Devuelve true si existe el fichero que se le pasa como parámetro
			- <code> fexists(fichero) </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// comprobar si existe el fichero config.xdl
					bool exists = viulib::System::fexists("config.xdl");
				\endcode
		*/
		static bool fexists(const char *filename);
		
		/** Devuelve la extensión del fichero que se le indica
			- <code> extension(fichero) </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// extensión del fichero "imagen.bmp"
					std::string ext = viulib::System::extension("imagen.bmp");
				\endcode
		*/
		static std::string extension(std::string file);

		/** Activa o desactiva un hook para el ratón
			- <code> listenMouse(enabled,callback) </code> .
			\pre
				
			\remark
				El valor de retorno le indicará al hook si seguir propagando el mensaje (0) o no (1)
				
			\par Ejemplo:
				\code
					// activar el hook para el ratón
					int callback_raton (cv::Point p, viulib::M_EVENT evento)
					{
					}
					.....

					viulib::System::listenMouse(true,&callback_raton);
				\endcode
		*/
		static 	void listenMouse(bool enabled,int (*callback)(cv::Point,systemLayer::M_EVENT));

		/** Activa o desactiva un hook para el teclado
			- <code> listenMouse(enabled,callback) </code> .
			\pre
				
			\remark
				El valor de retorno le indicará al hook si seguir propagando el mensaje (0) o no (1)
				El codigo que se le envía al callback es un byte 
				para caracteres imprimibles es su código ascii y para el resto un código definido en viulib
				estos códigos para otras teclas estan definidas como VIU_TECLA en viulib
				
			\par Ejemplo:
				\code
					// activar el hook para el ratón
					int callback_teclado (int code, viulib::K_EVENT evento)
					{
						if (code == VIU_LCONTROL) ...
						else if (code == (int)'A') ...
					}
					.....

					viulib::System::listenKeyboard(true,&callback_teclado);
				\endcode
		*/
		static 	void listenKeyboard(bool enabled,int (*callback)(int,systemLayer::K_EVENT));

		/** Función que nos indica si está o no pulsado el ALT cuando el hook está activo
			- <code> isAltPressed() </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// consultar la tecla ALT
					int callback_teclado (int code, viulib::K_EVENT evento)
					{
						if (code == VIU_LCONTROL) ...
						else if (code == (int)'A') ...
					}
					.....

					viulib::System::listenKeyboard(true,&callback_teclado);

					....
					viulib::isAltPressed();
				\endcode
		*/
		static 	bool isAltPressed();
		/** Función que nos indica si está o no pulsado el CTRL cuando el hook está activo
			- <code> isCtrlPressed() </code> .
			\pre
				
			\remark
				
			\par Ejemplo:
				\code
					// consultar la tecla CTRL
					int callback_teclado (int code, viulib::K_EVENT evento)
					{
						if (code == VIU_LCONTROL) ...
						else if (code == (int)'A') ...
					}
					.....

					viulib::System::listenKeyboard(true,&callback_teclado);

					....
					viulib::isCtrlPressed();
				\endcode
		*/
		static 	bool isCtrlPressed();
	};
	
};

#endif
