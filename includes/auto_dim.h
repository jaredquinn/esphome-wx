
void brightness_trans(float c) {
   if(c>1) { c=1; }
   if(c<.10) { c=.10; }
   auto call = id(back_light).turn_on();
   call.set_transition_length(2000);
   call.set_brightness(c);
   call.perform();
}

void change_adc(float a) {
   float b = float(id(bright_adjust).state)/100.00;
   float c = a * b;
   ESP_LOGI("bright/ldr", "a=%.2f, b=%.2f, c=%.2f", a, b, c);
   brightness_trans(c);
}

