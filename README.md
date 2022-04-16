# Omnis5K
The software associated with the 5K project running on the Omnis controller.

This project uses [PlatformIO](https://platformio.org/)

## Rocket States

Each general stage of flight is managed by a state machine.
The states of the system include:

1. Nav Converge
2. Pad Idle
3. Burn
4. Coast
5. Post Apogee
6. Chute Deployed
7. Grounded

