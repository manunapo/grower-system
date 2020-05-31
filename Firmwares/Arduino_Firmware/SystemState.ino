
// state = 0 -> desactivated
// state = 1 -> activated
int state = 0;

boolean changed = false;

boolean stateIsActivated(){
  return state == 1;
}

boolean stateIsDesactivated(){
  return state == 0;
}

boolean stateHasChanged(){
  return changed;
}

void stateActivate(){
  if(state == 0){
    state = 1;
    changed = true;
  }
}

void stateDesactivate(){
  if(state == 1){
    state = 0;
    changed = true;
  }
}

void stateRead(){
  changed = false;
}

