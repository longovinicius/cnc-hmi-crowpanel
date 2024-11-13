/**************************CrowPanel ESP32 HMI Display Example Code************************
Version     :	1.1
Suitable for:	CrowPanel ESP32 HMI Display
Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the course video: Introduction 
				        to ask questions or feedback.
**************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "ui.h"
// #include <demos/lv_demos.h>
// #include <examples/lv_examples.h>
/**************************LVGL and UI END************************/

/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"

struct InverterData {
    double speed;
    double current;
};

struct ZData {
    double speed;
    double height;
};

struct DrillData {
    ZData Z;
    InverterData inverter;
};

struct PumpData {
    bool pump1;
    bool pump2;
};

struct ManualData {
    int row;
    int hole;
};

struct MachineData {
    int mode;
    int currentRow;
    int currentHole;
    PumpData pumps;
    DrillData drill1;
    DrillData drill2;
    ManualData manual;
};

MachineData machineData;
bool dataUpdated = false;


static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;
void my_timer(lv_timer_t * timer);


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);

   lv_disp_flush_ready( disp );

}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando IHM");

  //GPIO init
#if defined (CrowPanel_50) || defined (CrowPanel_70)
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);

#elif defined (CrowPanel_43)
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);
  pinMode(35, OUTPUT);
  digitalWrite(35, LOW);
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(0, OUTPUT);//TOUCH-CS
#endif

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  delay(200);

  lv_init();

  delay(100);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight/10);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  tft.fillScreen(TFT_BLACK);

  //please do not use LVGL Demo and UI export from Squareline Studio in the same time.
  // lv_demo_widgets();    // LVGL demo
  ui_init();

  lv_timer_t * timer = lv_timer_create(my_timer, 200,  NULL);
  
  Serial.println( "Setup done" );

}

void updateMachineData(const JsonDocument& doc) {
  machineData.mode = doc["mode"].as<int>();
  machineData.currentRow = doc["row"].as<int>();
  machineData.currentHole = doc["hole"].as<int>();

  machineData.pumps.pump1 = doc["pump"]["1"].as<bool>();
  machineData.pumps.pump2 = doc["pump"]["2"].as<bool>();

  // Drill 1
  machineData.drill1.Z.speed = doc["drill"]["1"]["Z"]["speed"].as<double>();
  machineData.drill1.Z.height = doc["drill"]["1"]["Z"]["height"].as<double>();
  machineData.drill1.inverter.speed = doc["drill"]["1"]["inverter"]["speed"].as<double>();
  machineData.drill1.inverter.current = doc["drill"]["1"]["inverter"]["current"].as<double>();

  // Drill 2
  machineData.drill2.Z.speed = doc["drill"]["2"]["Z"]["speed"].as<double>();
  machineData.drill2.Z.height = doc["drill"]["2"]["Z"]["height"].as<double>();
  machineData.drill2.inverter.speed = doc["drill"]["2"]["inverter"]["speed"].as<double>();
  machineData.drill2.inverter.current = doc["drill"]["2"]["inverter"]["current"].as<double>();

  // Dados manuais
  machineData.manual.row = doc["manual"]["row"].as<int>();
  machineData.manual.hole = doc["manual"]["hole"].as<int>();

  dataUpdated = true;
}

void updateDisplay() {
    char buffer[64];

    // // Bombas
    // lv_label_set_text(ui_Pump1Label, machineData.pumps.pump1 ? "Ligada" : "Desligada");
    // lv_label_set_text(ui_Pump2Label, machineData.pumps.pump2 ? "Ligada" : "Desligada");

    // Modo
    sprintf(buffer, "Broca Atual: %d", machineData.mode);
    lv_label_set_text(ui_BrocaAtual, buffer);

    // Fileira e Contagem de Furos
    sprintf(buffer, "Fileira Atual: %d", machineData.currentRow);
    lv_label_set_text(ui_Fileira, buffer);
    lv_label_set_text(ui_FileiraAtual, buffer);
    sprintf(buffer, "Buraco Atual: %d", machineData.currentHole);
    lv_label_set_text(ui_Furo, buffer);
    lv_label_set_text(ui_FuroAtual, buffer);

    // Estimativa de Tempo Restante
    sprintf(buffer, "Tempo Restante? Onde calcular?: %d", 5);
    lv_label_set_text(ui_TempoEstimado, buffer);

    // Drill 1
    sprintf(buffer, "Velocidade Rotacao: %.2f [RPM]", machineData.drill1.Z.speed);
    lv_label_set_text(ui_Avanco1, buffer);
    sprintf(buffer, "Posicao: %.2f [mm]", machineData.drill1.Z.height);
    lv_label_set_text(ui_Posicao1, buffer);
    sprintf(buffer, "Velocidade Avanco: %.2f [mm/s]", machineData.drill1.inverter.speed);
    lv_label_set_text(ui_Motor1, buffer);
    sprintf(buffer, "Corrente Inversor: %.2f [A]", machineData.drill1.inverter.current);
    lv_label_set_text(ui_Inversor1, buffer);

    // Drill 2
    sprintf(buffer, "Velocidade Rotacao: %.2f [RPM]", machineData.drill2.Z.speed);
    lv_label_set_text(ui_Avanco2, buffer);
    sprintf(buffer, "Posicao: %.2f [mm]", machineData.drill2.Z.height);
    lv_label_set_text(ui_Posicao2, buffer);
    sprintf(buffer, "Velocidade Avanco: %.2f [mm/s]", machineData.drill2.inverter.speed);
    lv_label_set_text(ui_Motor2, buffer);
    sprintf(buffer, "Corrente Inversor: %.2f [A]", machineData.drill2.inverter.current);
    lv_label_set_text(ui_Inversor2, buffer);

    // Dados manuais
    sprintf(buffer, "Fileira Alvo: %d", machineData.manual.row);
    lv_label_set_text(ui_FileiraDestino, buffer);
    sprintf(buffer, "Buraco Alvo: %d", machineData.manual.hole);
    lv_label_set_text(ui_FuroDestino, buffer);
}

void my_timer(lv_timer_t * timer) {
    if (dataUpdated) {
        Serial.println("UPDATING DISPLAY");
        updateDisplay();
        dataUpdated = false; // Reseta a flag após atualizar o display
    }
}


void loop() {
  if (Serial.available()) {
    String json_str = Serial.readStringUntil('\n');
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, json_str);

    if (!error) {
      Serial.println("STORING DATA INTO MachineData");
      updateMachineData(doc);
      // dataUpdated é setada para true dentro de updateMachineData()
    } else {
      Serial.print("ERRO AO DESERIALIZAR JSON: ");
      Serial.println(error.c_str());
    }
  }
  else{
    Serial.println("No data in UART0");
  }

  lv_timer_handler(); // Necessário para o LVGL
  delay(5);
}