// manifest.h
// Master object manifest — declares known object categories, types, and roles.
// This file is a reference document only and not intended to be compiled.

#ifndef MANIFEST_H
#define MANIFEST_H

/*
============================================
🧱 BaseObject
- The root of all object types
============================================
*/
// Properties:
//   - const char* name
//   - const char* text (optional)
//   - const char* font (optional)
//   - const uint8_t* bitmap (optional)

/*
============================================
🖥️ ContextObject : BaseObject
- Represents a screen or interaction mode
============================================
*/
// Examples:
//   - BOOT
//   - MAIN_MENU
//   - SETTINGS
//   - PATTERN_MENU
//   - LIVE_MODE
//   - SAVE_MENU
//   - CALIBRATION *Will be in DEBUG*
//   - DEBUG

// Properties:
//   - const char* parentName
//   - const char** subcontextNames
//   - uint8_t subcontextCount
//   - bool backCapable
//   - draw(), update(), handleInput(), output()

/*
============================================
📜 MenuObject : ContextObject
- A selectable menu with toggleable items
============================================
*/
// Properties:
//   - const char** items
//   - uint8_t itemCount
//   - uint8_t selectedIndex
//   - bool* toggleStates
//   - onSelect(), onToggle()

/*
============================================
🎛️ InputObject : BaseObject
- Represents a physical or logical input
============================================
*/
// Types:
//   - Button
//   - Encoder
//   - Potentiometer
//   - ExtClock
//   - Serial
//   - Keyboard

// Properties:
//   - int pin
//   - bool isEncoder
//   - Input handling logic is defined by current ContextObject

/*
============================================
💡 OutputObject : BaseObject
- Represents a physical or logical output
============================================
*/
// Types:
//   - Screen
//   - Lights
//   - Sounds
//   - Triggers

// Properties:
//   - int pin
//   - void set(bool state)

/*
============================================
⚙️ StateObject : BaseObject
- Stores current system state or settings
============================================
*/
// Common Examples:
//   - currentPattern
//   - tempo
//   - swing
//   - screenBrightness
//   - otherValues[] (for general config)

/*
============================================
🎬 ActionObject : BaseObject
- Represents callable behavior
============================================
*/
// Built-in Actions:
//   - save, load, erase
//   - reset, home
//   - start, stop
//   - up, down, left
//   - toggleValue, yes, no
//   - other (custom-defined)

// Method:
//   - void execute()

/*
============================================
📚 RegistryObject : BaseObject
- Holds references to all registered objects
============================================
*/
// Note: this will be broken into specific registries such as:
//   - context_registry.h/.cpp
//   - input_registry.h/.cpp
//   - etc.

#endif // MANIFEST_H
