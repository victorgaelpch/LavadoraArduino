# ¿Que puede hacer?
Actualmente creo ya esta implementada toda la funcionalidad de una lavadora lo unico falta afinar es lo del nivel del agua
# Datos importantes
El archivo "webpageCode.h" contiene el HTML de la página. La verdad es que todavía no sé exactamente cuáles son las limitaciones del ESP32 en cuanto a lo compleja que puede ser la página, pero bueno, para implementar algún cambio de diseño, está el archivo "index.html", que es la misma página pero para que la modifiques. Para agregar la nueva modificación, simplemente debes copiar y pegar dentro de const char webpageCode[] = R"=====( *pegue aquí su código HTML, CSS, JS *)=====";

Tambien quiero aclarar que las funciones y tiempos de mi codigo son para mi lavadora pero lo hice lo mas dinamico posible para que se adapte a cualquier lavadora con un minimo de cambios, nota* la funcion tambien del llenado se puede controlar con el presostato de la lavadora pero la mia no le sirve asi que tendran que implementar

# Vista previa de la pagina web 
   Ayuda no se centrar divs T-T
   
![Captura de pantalla 2023-09-07 001157](https://github.com/victorgaelpch/LavadoraArduino/assets/80434602/73dafd78-bfae-46c4-868e-73f925b44c6f)

![Captura de pantalla 2023-09-07 001208](https://github.com/victorgaelpch/LavadoraArduino/assets/80434602/0843df65-a500-4a8c-8bda-10a3e6f9758a)
