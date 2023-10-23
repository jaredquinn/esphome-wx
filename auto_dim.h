
void brightness_trans(float c) {
   if(c>1) { c=1; }
   if(c<.10) { c=.10; }
   auto call = id(back_light).turn_on();
   call.set_transition_length(2000);
   call.set_brightness(c);
   call.perform();
}
