# Controller Layer – Event Mediation & Glue Logic

The controller connects input events from the GUI or OS to the emulator model.

## Components
- **Input Driver**  
  Captures raw key states or controller input.

- **Event Handler**  
  Translates GUI or keyboard events into emulator-specific actions.

- **Glue Logic**  
  Coordinates communication between model and view layers.

## Responsibilities
- Poll input states and update memory-mapped ports.
- Route events such as "Reset", "Pause", "Load ROM", or "Step".
- Handle bidirectional logic between GUI and emulator core.
