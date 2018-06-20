#ifndef SETTINGS_H
#define SETTINGS_H

/* Platform options */
#define RPI         0
#define PC_ALLEGRO  1

#define PLATFORM_MODE PC_ALLEGRO

#if ((PLATFORM_MODE != RPI) && (PLATFORM_MODE != PC_ALLEGRO))
#error "No se encuentra seleccionada una plataforma valida"
#endif

#endif /* SETTINGS_H */

