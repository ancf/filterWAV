// Następujący blok ifdef jest standardowym sposobem tworzenia makr, które powodują, że eksportowanie
// z biblioteki DLL jest prostsze. Wszystkie pliki w obrębie biblioteki DLL są kompilowane z LIBRARYC_EXPORTS
#ifdef LIBRARYC_EXPORTS
#define LIBRARYC_API __declspec(dllexport)
#else
#define LIBRARYC_API __declspec(dllimport)
#endif



extern "C" LIBRARYC_API void FilterC(double * tab, long min, long max);

