#ifndef SETTINGS_H
#define SETTINGS_H

/* Platform options */
#define RPI 
#define PC_ALLEGRO

#define PLATFORM_MODE RPI

#if ((PLATFORM_MODE != RPI) || (PLATFORM_MODE != PC_ALLEGRO))
#error "No se encuentra seleccionada una plataforma valida"
#endif /* SETTINGS_H */

