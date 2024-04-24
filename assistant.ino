';'
#include <Picovoice_EN.h>
#include <RGBLed.h>
#include "params.h"

RGBLed rgb(LEDR, LEDG, LEDB, RGBLed::COMMON_ANODE);
#define MEMORY_BUFFER_SIZE (70 * 1024)

static const char *ACCESS_KEY = "<your access key>";  // AccessKey string obtained from Picovoice Console (https://picovoice.ai/console/)

static pv_picovoice_t *handle = NULL;

String intentStr = "", slotVals[10];

static int8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static const float PORCUPINE_SENSITIVITY = 0.75f;
static const float RHINO_SENSITIVITY = 0.5f;
static const float RHINO_ENDPOINT_DURATION_SEC = 1.0f;
static const bool RHINO_REQUIRE_ENDPOINT = true;

static void wake_word_callback(void) {
  Serial.println("Wake word detected!");
  rgb.crossFade(0, 200, 255, 255, 50, 0, 30, 300);
  delay(100);
  digitalWrite(LED_BUILTIN, 1);
  rgb.fadeOut(255, 50, 0, 20, 100);
}

static void inference_callback(pv_inference_t *inference) {
  intentStr = "";
  Serial.println("{");
  Serial.print("    is_understood : ");
  if (inference->is_understood) {
    rgb.flash(RGBLed::GREEN, 80, 100);
    rgb.flash(RGBLed::GREEN, 80, 100);
    Serial.print("true\n");
  } else {
    rgb.flash(RGBLed::RED, 80, 100);
    rgb.flash(RGBLed::RED, 80, 100);
    Serial.print("false\n");
  }
  if (inference->is_understood) {
    Serial.print("    intent : ");
    intentStr = inference->intent;
    Serial.println(intentStr);
    if (inference->num_slots > 0) {
      Serial.println("    slots : {");
      int j = 0;
      for (int32_t i = 0; i < inference->num_slots; i++) {
        Serial.print("        ");
        Serial.print(inference->slots[i]);
        slotVals[j] = String(inference->slots[i]);
        Serial.print(" : ");
        Serial.println(inference->values[i]);
        slotVals[j + 1] = String(inference->values[i]);
        j += 2;
      }
      Serial.println("    }");
    }
  }
  Serial.println("}\n");
  /*
  Serial.println(intentStr);
  for (int32_t i = 0; i < 2 * inference->num_slots; i += 2) {
    Serial.print(slotVals[i]);
    Serial.print(" ");
    Serial.println(slotVals[i + 1]);
  }
  */
  for (int32_t i = 0; i < 2 * inference->num_slots; i += 2) {
    if (intentStr == "changeLightState") {
      if (slotVals[i] == "state") {
        if (slotVals[i + 1] == "on") {
          rgb.fadeIn(255, 255, 255, 20, 100);
        } 
        else if (slotVals[i + 1] == "off") {
          rgb.fadeOut(0, 100, 255, 20, 100);
        }
      }
    }

    if (intentStr == "changeColor") {
      if (slotVals[i] == "color") {
        if (slotVals[i + 1] == "blue")
          rgb.setColor(RGBLed::BLUE);
        else if (slotVals[i + 1] == "green")
          rgb.setColor(RGBLed::GREEN);
        else if (slotVals[i + 1] == "orange")
          rgb.setColor(255, 100, 0);
        else if (slotVals[i + 1] == "pink")
          rgb.setColor(255, 50, 200);
        else if (slotVals[i + 1] == "purple")
          rgb.setColor(255, 0, 255);
        else if (slotVals[i + 1] == "red")
          rgb.setColor(RGBLed::RED);
        else if (slotVals[i + 1] == "white")
          rgb.setColor(RGBLed::WHITE);
        else if (slotVals[i + 1] == "yellow")
          rgb.setColor(RGBLed::YELLOW);
      }
    }
  }

  pv_inference_delete(inference);
  digitalWrite(LED_BUILTIN, 0);
}

static void print_error_message(char **message_stack, int32_t message_stack_depth) {
  for (int32_t i = 0; i < message_stack_depth; i++) {
    Serial.println(message_stack[i]);
  }
}

void setup() {
  Serial.begin(115200);
  rgb.off();
  while (!Serial)
    ;

  pv_status_t status = pv_audio_rec_init();
  if (status != PV_STATUS_SUCCESS) {
    Serial.print("Audio init failed with ");
    Serial.println(pv_status_to_string(status));
    while (1)
      ;
  }

  char **message_stack = NULL;
  int32_t message_stack_depth = 0;
  pv_status_t error_status;

  status = pv_picovoice_init(
    ACCESS_KEY,
    MEMORY_BUFFER_SIZE,
    memory_buffer,
    sizeof(KEYWORD_ARRAY),
    KEYWORD_ARRAY,
    PORCUPINE_SENSITIVITY,
    wake_word_callback,
    sizeof(CONTEXT_ARRAY),
    CONTEXT_ARRAY,
    RHINO_SENSITIVITY,
    RHINO_ENDPOINT_DURATION_SEC,
    RHINO_REQUIRE_ENDPOINT,
    inference_callback,
    &handle);
  if (status != PV_STATUS_SUCCESS) {
    Serial.print("Picovoice init failed with ");
    Serial.println(pv_status_to_string(status));

    error_status = pv_get_error_stack(&message_stack, &message_stack_depth);
    if (error_status != PV_STATUS_SUCCESS) {
      Serial.println("Unable to get Porcupine error state");
      while (1)
        ;
    }
    print_error_message(message_stack, message_stack_depth);
    pv_free_error_stack(message_stack);

    while (1)
      ;
  }

  const char *rhino_context = NULL;
  status = pv_picovoice_context_info(handle, &rhino_context);
  if (status != PV_STATUS_SUCCESS) {
    Serial.print("retrieving context info failed with");
    Serial.println(pv_status_to_string(status));
    while (1)
      ;
  }
  Serial.println("Wake word: 'hey computer'");
  Serial.println(rhino_context);
}

void loop() {
  const int16_t *buffer = pv_audio_rec_get_new_buffer();
  if (buffer) {
    const pv_status_t status = pv_picovoice_process(handle, buffer);
    if (status != PV_STATUS_SUCCESS) {
      Serial.print("Picovoice process failed with ");
      Serial.println(pv_status_to_string(status));
      char **message_stack = NULL;
      int32_t message_stack_depth = 0;
      pv_get_error_stack(
        &message_stack,
        &message_stack_depth);
      for (int32_t i = 0; i < message_stack_depth; i++) {
        Serial.println(message_stack[i]);
      }
      pv_free_error_stack(message_stack);
      while (1)
        ;
    }
  }
}
