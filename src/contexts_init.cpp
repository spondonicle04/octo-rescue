#include "contexts_init.h"
#include "menu_main.h"
#include "menu_settings.h"
#include "menu_live.h"
#include "menu_pattern.h"
#include "menu_save.h"
#include "menu_debug.h"

extern void registerMainMenuContext();
extern void registerSettingsMenuContext();
extern void registerLiveModeContext();
extern void registerPatternMenuContext();
extern void registerSaveMenuContext();
extern void registerDebugMenuContext();

void registerAllContexts() {
  registerMainMenuContext();
  registerSettingsMenuContext();
  registerLiveModeContext();
  registerPatternMenuContext();
  registerSaveMenuContext();
  registerDebugMenuContext();
}
