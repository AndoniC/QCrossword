// ==============================================================================================
// MACRO FOR IMPORTING AND EXPORTING FUNCTIONS AND CLASSES FROM DLL
// ==============================================================================================
// When the symbol BOT_EXPORTS is defined in a project OMF exports functions and classes. 
// In other cases BOT_LIB imports them from the DLL
#ifdef systemlayer_EXPORTS
	#if defined _WIN32 || defined _WIN64
		#define SL_LIB __declspec( dllexport )
	#else
		#define SL_LIB
	#endif
#else
	#if defined _WIN32 || defined _WIN64
		#define SL_LIB __declspec( dllimport )
	#else
		#define SL_LIB
	#endif
#endif

// ==============================================================================================
// AUTOMATIC DLL LINKAGE
// ==============================================================================================
// If the code is compiled under MS VISUAL STUDIO, and BOT_EXPORTS is not defined (i.e.
// in a DLL client program) this code will link the appropiate lib file, either in DEBUG or in
// RELEASE
#if defined( _MSC_VER ) && !defined( systemlayer_EXPORTS )
	#ifdef _DEBUG
		#pragma comment( lib, "systemlayer_d.lib" )
	#else
		#pragma comment( lib, "systemlayer.lib" )
	#endif
#endif

