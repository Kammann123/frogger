#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/gui/allegro/allegro_init.o \
	${OBJECTDIR}/gui/allegro/allegro_input/allegro_input.o \
	${OBJECTDIR}/gui/allegro/frogger/changescreen/allegro_frogger_changescreen.o \
	${OBJECTDIR}/gui/allegro/frogger/game/allegro_frogger_game.o \
	${OBJECTDIR}/gui/allegro/frogger/lostscreen/allegro_frogger_lostscreen.o \
	${OBJECTDIR}/gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.o \
	${OBJECTDIR}/gui/allegro/frogger/pausemenu/allegro_frogger_pausemenu.o \
	${OBJECTDIR}/gui/frogger/frogger_changescreen/frogger_changescreen.o \
	${OBJECTDIR}/gui/frogger/frogger_game/frogger_game.o \
	${OBJECTDIR}/gui/frogger/frogger_lostscreen/frogger_lostscreen.o \
	${OBJECTDIR}/gui/frogger/frogger_mainmenu/frogger_mainmenu.o \
	${OBJECTDIR}/gui/frogger/frogger_pausemenu/frogger_pausemenu.o \
	${OBJECTDIR}/gui/gui_animation/gui_animation.o \
	${OBJECTDIR}/gui/gui_events/gui_events.o \
	${OBJECTDIR}/gui/gui_files/gui_files.o \
	${OBJECTDIR}/gui/gui_init.o \
	${OBJECTDIR}/gui/gui_input/gui_input.o \
	${OBJECTDIR}/gui/gui_timer/gui_timer.o \
	${OBJECTDIR}/gui/gui_types.o \
	${OBJECTDIR}/logic/frogger/frogger_kernel.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/timer.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/frogger

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/frogger: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/frogger ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/gui/allegro/allegro_init.o: gui/allegro/allegro_init.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/allegro_init.o gui/allegro/allegro_init.c

${OBJECTDIR}/gui/allegro/allegro_input/allegro_input.o: gui/allegro/allegro_input/allegro_input.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/allegro_input
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/allegro_input/allegro_input.o gui/allegro/allegro_input/allegro_input.c

${OBJECTDIR}/gui/allegro/frogger/changescreen/allegro_frogger_changescreen.o: gui/allegro/frogger/changescreen/allegro_frogger_changescreen.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/frogger/changescreen
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/frogger/changescreen/allegro_frogger_changescreen.o gui/allegro/frogger/changescreen/allegro_frogger_changescreen.c

${OBJECTDIR}/gui/allegro/frogger/game/allegro_frogger_game.o: gui/allegro/frogger/game/allegro_frogger_game.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/frogger/game
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/frogger/game/allegro_frogger_game.o gui/allegro/frogger/game/allegro_frogger_game.c

${OBJECTDIR}/gui/allegro/frogger/lostscreen/allegro_frogger_lostscreen.o: gui/allegro/frogger/lostscreen/allegro_frogger_lostscreen.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/frogger/lostscreen
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/frogger/lostscreen/allegro_frogger_lostscreen.o gui/allegro/frogger/lostscreen/allegro_frogger_lostscreen.c

${OBJECTDIR}/gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.o: gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/frogger/mainmenu
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.o gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.c

${OBJECTDIR}/gui/allegro/frogger/pausemenu/allegro_frogger_pausemenu.o: gui/allegro/frogger/pausemenu/allegro_frogger_pausemenu.c 
	${MKDIR} -p ${OBJECTDIR}/gui/allegro/frogger/pausemenu
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/allegro/frogger/pausemenu/allegro_frogger_pausemenu.o gui/allegro/frogger/pausemenu/allegro_frogger_pausemenu.c

${OBJECTDIR}/gui/frogger/frogger_changescreen/frogger_changescreen.o: gui/frogger/frogger_changescreen/frogger_changescreen.c 
	${MKDIR} -p ${OBJECTDIR}/gui/frogger/frogger_changescreen
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/frogger/frogger_changescreen/frogger_changescreen.o gui/frogger/frogger_changescreen/frogger_changescreen.c

${OBJECTDIR}/gui/frogger/frogger_game/frogger_game.o: gui/frogger/frogger_game/frogger_game.c 
	${MKDIR} -p ${OBJECTDIR}/gui/frogger/frogger_game
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/frogger/frogger_game/frogger_game.o gui/frogger/frogger_game/frogger_game.c

${OBJECTDIR}/gui/frogger/frogger_lostscreen/frogger_lostscreen.o: gui/frogger/frogger_lostscreen/frogger_lostscreen.c 
	${MKDIR} -p ${OBJECTDIR}/gui/frogger/frogger_lostscreen
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/frogger/frogger_lostscreen/frogger_lostscreen.o gui/frogger/frogger_lostscreen/frogger_lostscreen.c

${OBJECTDIR}/gui/frogger/frogger_mainmenu/frogger_mainmenu.o: gui/frogger/frogger_mainmenu/frogger_mainmenu.c 
	${MKDIR} -p ${OBJECTDIR}/gui/frogger/frogger_mainmenu
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/frogger/frogger_mainmenu/frogger_mainmenu.o gui/frogger/frogger_mainmenu/frogger_mainmenu.c

${OBJECTDIR}/gui/frogger/frogger_pausemenu/frogger_pausemenu.o: gui/frogger/frogger_pausemenu/frogger_pausemenu.c 
	${MKDIR} -p ${OBJECTDIR}/gui/frogger/frogger_pausemenu
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/frogger/frogger_pausemenu/frogger_pausemenu.o gui/frogger/frogger_pausemenu/frogger_pausemenu.c

${OBJECTDIR}/gui/gui_animation/gui_animation.o: gui/gui_animation/gui_animation.c 
	${MKDIR} -p ${OBJECTDIR}/gui/gui_animation
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_animation/gui_animation.o gui/gui_animation/gui_animation.c

${OBJECTDIR}/gui/gui_events/gui_events.o: gui/gui_events/gui_events.c 
	${MKDIR} -p ${OBJECTDIR}/gui/gui_events
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_events/gui_events.o gui/gui_events/gui_events.c

${OBJECTDIR}/gui/gui_files/gui_files.o: gui/gui_files/gui_files.c 
	${MKDIR} -p ${OBJECTDIR}/gui/gui_files
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_files/gui_files.o gui/gui_files/gui_files.c

${OBJECTDIR}/gui/gui_init.o: gui/gui_init.c 
	${MKDIR} -p ${OBJECTDIR}/gui
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_init.o gui/gui_init.c

${OBJECTDIR}/gui/gui_input/gui_input.o: gui/gui_input/gui_input.c 
	${MKDIR} -p ${OBJECTDIR}/gui/gui_input
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_input/gui_input.o gui/gui_input/gui_input.c

${OBJECTDIR}/gui/gui_timer/gui_timer.o: gui/gui_timer/gui_timer.c 
	${MKDIR} -p ${OBJECTDIR}/gui/gui_timer
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_timer/gui_timer.o gui/gui_timer/gui_timer.c

${OBJECTDIR}/gui/gui_types.o: gui/gui_types.c 
	${MKDIR} -p ${OBJECTDIR}/gui
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gui/gui_types.o gui/gui_types.c

${OBJECTDIR}/logic/frogger/frogger_kernel.o: logic/frogger/frogger_kernel.c 
	${MKDIR} -p ${OBJECTDIR}/logic/frogger
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/logic/frogger/frogger_kernel.o logic/frogger/frogger_kernel.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/timer.o: timer.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/timer.o timer.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/frogger

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
