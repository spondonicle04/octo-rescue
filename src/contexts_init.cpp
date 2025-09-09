#include "contexts_init.h"
#include "menu_main.h"
#include "menu_settings.h"
#include "menu_live.h"
#include "menu_pattern.h"
#include "menu_save.h"
#include "menu_debug.h"
#include "menu_display.h"
#include "menu_led.h"
#include "menu_boot.h"

extern void registerMainMenuContext();
extern void registerSettingsMenuContext();
extern void registerLiveModeContext();
extern void registerPatternMenuContext();
extern void registerSaveMenuContext();
extern void registerDebugMenuContext();
extern void registerSystemInfoContext();
extern void registerTestBeepContext();
extern void registerDisplayOptionsContext();
extern void registerDisplayBrightnessContext();
extern void registerDisplayInvertContext();
extern void registerLedOptionsContext();
extern void registerLedBrightnessContext();
extern void registerLedHitColorContext();
extern void registerLedStepColorContext();
extern void registerBootContext();

void registerAllContexts() {
  // Boot/splash (optional)
  registerBootContext();

  registerMainMenuContext();
  registerSettingsMenuContext();
  registerLiveModeContext();
  registerPatternMenuContext();
  registerSaveMenuContext();
  registerDebugMenuContext();

  // Debug sub-screens
  registerSystemInfoContext();
  registerTestBeepContext();

  // Display settings and subs
  registerDisplayOptionsContext();
  registerDisplayBrightnessContext();
  registerDisplayInvertContext();

  // LED settings and subs
  registerLedOptionsContext();
  registerLedBrightnessContext();
  registerLedHitColorContext();
  registerLedStepColorContext();
}
