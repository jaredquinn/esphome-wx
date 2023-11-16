
esphome::display::BaseFont* FONT_TITLE;
esphome::display::BaseFont* FONT_CLOCK;
esphome::display::BaseFont* FONT_VALUE_BIG;
esphome::display::BaseFont* FONT_VALUE_SMALL;
esphome::display::BaseFont* FONT_VALUE;
esphome::display::BaseFont* FONT_LABELS;
esphome::display::BaseFont* FONT_MDI;
esphome::display::BaseFont* FONT_COMPASS;

void df_button(esphome::display::Display &lcd, int left, int top, int width, int height, const char* text, bool state) {
  auto bg = id(button_bg);
  auto fg = Color::WHITE;
  if(state) { fg = id(button_bg); bg = Color::BLACK; }
  lcd.filled_rectangle(left, top, width, height, bg);
  lcd.printf(left+int(width/2), top+int(height/2), FONT_LABELS, fg, TextAlign::CENTER, "%s", text);
}
void df_horizontal_bar(esphome::display::Display &lcd, int top, esphome::Color lineColor) {
  lcd.line(0, top, lcd.get_width(), top, lineColor);
}
void df_boot_screen(esphome::display::Display &lcd) {
  int center=lcd.get_width()/2;
  lcd.print(center, 80, FONT_VALUE_BIG, id(my_red), TextAlign::TOP_CENTER, "esphomeWX");
  lcd.print(center, 160, FONT_VALUE, TextAlign::TOP_CENTER, "Waiting for Connection");
}

void df_display_calibration_data(esphome::display::Display &lcd, int top) {
  int colwidth=lcd.get_width()/4;
  lcd.printf(0, top, FONT_LABELS, TextAlign::TOP_LEFT, "x=%d", id(lastX));
  lcd.printf(colwidth*1, top, FONT_LABELS, TextAlign::TOP_LEFT, "y=%d", id(lastY));
  lcd.printf(colwidth*2, top, FONT_LABELS, TextAlign::TOP_LEFT, "rx=%d", id(lastXraw));
  lcd.printf(colwidth*3, top, FONT_LABELS, TextAlign::TOP_LEFT, "ry=%d", id(lastYraw));
  lcd.filled_rectangle(id(lastX), id(lastY), 5, 5, id(my_red));
}

void df_top_bar(esphome::display::Display &lcd, const char *text)
{
  lcd.filled_rectangle(0, 0, lcd.get_width(), 24, id(bar_bg));
  lcd.print(5, 3, FONT_TITLE, id(my_yellow), TextAlign::TOP_LEFT, text);
  if(id(nettime).now().is_valid()) {
    lcd.strftime(lcd.get_width()-5, 3, FONT_TITLE, id(my_yellow), TextAlign::TOP_RIGHT, "%a %d %b %Y", id(nettime).now());
  }
}

void df_clock_widget(esphome::display::Display &lcd, int left, int top, int right, int bottom) 
{
   lcd.start_clipping(left, top, right, bottom);
   lcd.strftime(left, top,  FONT_CLOCK, id(color_clock), TextAlign::TOP_LEFT, "%H:%M", id(nettime).now());
   if(id(nettime).now().second % 2 == 0) {
     lcd.print(left+56, top, FONT_CLOCK, Color::BLACK, TextAlign::TOP_LEFT, ":");
   }
   lcd.end_clipping();
}

void df_big_temphumid(esphome::display::Display &lcd, int left, int top, int right, int bottom, float bigValue, float smallValue, const char* label) {
   lcd.start_clipping(left, top, right, bottom);
   lcd.printf(left+4, top,  FONT_VALUE_BIG, TextAlign::TOP_LEFT, "%.0f°", bigValue);
   lcd.printf(left+4, top+26, FONT_LABELS,  id(color_labels), TextAlign::TOP_LEFT, label);
   lcd.printf(left+4, top+34, FONT_VALUE_SMALL, TextAlign::TOP_LEFT, "%.0f %%", smallValue);
   lcd.end_clipping();
}

void df_icon_left(esphome::display::Display &lcd, int left, int top, int right, int bottom, const char* topLabel, const char* bottomLabel, esphome::display::BaseFont* iconFont, const char *icon, esphome::Color iconColor, const char *format, float value)
{
   lcd.printf(right-32, top, FONT_VALUE_BIG, TextAlign::TOP_RIGHT, format, value);
   lcd.print(left+2, top+2,  iconFont, iconColor, TextAlign::TOP_LEFT, icon);
   lcd.print(right-4, top+2,  FONT_LABELS, id(color_labels), TextAlign::TOP_RIGHT, topLabel);
   lcd.print(right-4, top+14, FONT_LABELS, id(color_labels),TextAlign::TOP_RIGHT, bottomLabel);
}

void df_small_temphum(esphome::display::Display &lcd, int left, int top, int right, int bottom, 
	const char *label, const char *bigformat, esphome::Color bigcolor, float bigvalue, const char *smallformat, esphome::Color smallcolor, float smallvalue) {
   lcd.printf(left, top+24, FONT_LABELS,  id(color_labels), TextAlign::TOP_LEFT, label);
   lcd.printf(left, top+2, FONT_VALUE, bigcolor, TextAlign::TOP_LEFT, bigformat, bigvalue);
   lcd.printf(left, top+32, FONT_VALUE_SMALL, smallcolor, TextAlign::TOP_LEFT, smallformat, smallvalue);
}

void df_big_value(esphome::display::Display &lcd, int left, int top, int right, int bottom, 
	const char *toplabel, const char *bottomlabel, esphome::Color color, const char *format, float value) {
   lcd.printf(left, top+4, FONT_LABELS,  id(color_labels), TextAlign::TOP_LEFT, toplabel);
   lcd.printf(left, top+42, FONT_LABELS,  id(color_labels), TextAlign::TOP_LEFT, bottomlabel);
   lcd.printf(left, top+14, FONT_VALUE_BIG, color, TextAlign::TOP_LEFT, format, value);
}

