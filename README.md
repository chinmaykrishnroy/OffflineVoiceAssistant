# Offline Voice Assistant with Picovoice and Edge ML on Arduino Nano BLE Sense

This repository contains the code for an offline voice assistant implemented with Picovoice technology and Edge ML capabilities on an Arduino Nano BLE Sense. The voice assistant allows users to control an RGB LED using voice commands, leveraging the power of machine learning at the edge for efficient and privacy-preserving inference.

## Features

- **Picovoice Voice Recognition**: Offline voice recognition technology for accurate and efficient voice commands.
- **Edge ML Integration**: Machine learning at the edge enables powerful inference capabilities on constrained devices.
- **Voice-Controlled RGB LED**: Control the RGB LED using natural voice commands, enhancing user interaction.
- **Privacy-Preserving**: Perform inference entirely offline, ensuring data privacy and security.
- **Modular Architecture**: Easily extend or modify the assistant's functionality with additional voice commands or machine learning models.
- **Low Resource Consumption**: Efficient use of resources enables seamless operation on resource-constrained devices.
- **Real-Time Inference**: Instantaneous response to voice commands with minimal latency, providing a smooth user experience.

## Requirements

- Arduino Nano BLE Sense board
- RGB LED connected to the appropriate pins on the board
- Picovoice Access Key obtained from the Picovoice Console
- Arduino IDE or compatible development environment for code compilation and uploading

## Getting Started

1. **Setup Hardware**:
   - Connect the RGB LED to the specified pins on the Arduino Nano BLE Sense board.

2. **Obtain Picovoice Access Key**:
   - Sign up for an account on the [Picovoice Console](https://picovoice.ai/console/) to obtain an Access Key.

3. **Configure Code**:
   - Replace the `ACCESS_KEY` variable in the code with your Picovoice Access Key.
   - Integrate the Edge ML model into the code for additional inference capabilities.
   - Customize voice commands and LED control logic as needed.

4. **Upload Code**:
   - Upload the provided sketch to the Arduino Nano BLE Sense board using Arduino IDE or a compatible development environment.

5. **Interact with the Voice Assistant**:
   - Speak the wake word to activate the voice assistant.
   - Issue voice commands to control the RGB LED, observing real-time response and inference accuracy.
  
## Image
![assistant](https://github.com/chinmaykrishnroy/OffflineVoiceAssistant/assets/65699140/ef2e8e42-b01f-4f37-a508-2b4481b59f9d)


## License

This project is licensed under the Apache License 2.0. See the [LICENSE](LICENSE) file for details.
