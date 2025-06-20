// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project0

#include "ui.h"

void ui_Pagina_de_Emergencia_screen_init(void)
{
    ui_Pagina_de_Emergencia = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Pagina_de_Emergencia, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Pagina_de_Emergencia, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Pagina_de_Emergencia, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Pagina_de_Emergencia, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Pagina_de_Emergencia, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Pagina_de_Emergencia, 25, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelEmergencia = lv_label_create(ui_Pagina_de_Emergencia);
    lv_obj_set_width(ui_LabelEmergencia, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelEmergencia, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelEmergencia, 8);
    lv_obj_set_y(ui_LabelEmergencia, -171);
    lv_obj_set_align(ui_LabelEmergencia, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelEmergencia, "EMERGENCIA");
    lv_obj_set_style_text_font(ui_LabelEmergencia, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelEmergencia, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelEmergencia, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_LabelEmergencia, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_LabelEmergencia, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelInstrucoesEmergencia = lv_label_create(ui_Pagina_de_Emergencia);
    lv_obj_set_width(ui_LabelInstrucoesEmergencia, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelInstrucoesEmergencia, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelInstrucoesEmergencia, 1);
    lv_obj_set_y(ui_LabelInstrucoesEmergencia, 32);
    lv_obj_set_align(ui_LabelInstrucoesEmergencia, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelInstrucoesEmergencia,
                      "1) Cheque se a area de furacao esta livre\ne a broca esta instalada corretamente\n\n2) Feche a porta\n\n3) Aperte o botao de RESET");
    lv_obj_set_style_text_font(ui_LabelInstrucoesEmergencia, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);

}
