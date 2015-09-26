/**
 * definitions for update
 */

#ifndef INCLUDE_UPDATE_H_
#define INCLUDE_UPDATE_H_

/**
 * update application
 */

void ICACHE_FLASH_ATTR update_app(Print * messages, bool includeFiles);


/**
 * update files
 */
void ICACHE_FLASH_ATTR update_files(Stream & messages);


/**
 * switch roms
 */
void ICACHE_FLASH_ATTR update_switch_roms(Stream & messages);

/**
 * check rboot config, and adopt rom positions if needed; should happen in build process!!
 */
void ICACHE_FLASH_ATTR update_check_rboot_config();

/**
 * print out config
 */
void ICACHE_FLASH_ATTR update_print_config(Print* out);

#endif /* INCLUDE_UPDATE_H_ */
