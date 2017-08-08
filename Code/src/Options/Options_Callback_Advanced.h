#ifndef OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER
#define OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Security/Authentification.h"
#include "../Hardware/SelfTest.h"
#include "../FSM/States/State_Recovery.h"

#define opt_callback_change_key change_master_key
void opt_callback_show_key(void);
void opt_callback_inverse_screen(void);
void opt_callback_change_keyboard(void);
#define opt_callback_self_test self_test_execute
#define opt_callback_full_reset state_recovery_do_full_reset


#endif // OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER
