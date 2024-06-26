#include "CPdoMapping.hpp"

int CPdoMapping::wkc;
int CPdoMapping::retVal;

/** SOEM wrapper for ec_SDOwrite().
 *
 * @param slaveIdx     = Slave index.
 * @param index     = SDO index.
 * @param subindex  = SDO sub-index.
 * @param value     = Output value.
 * @return working counter.
 */
int CPdoMapping::SDO_write8(uint16 slaveIdx, uint16 index, uint8 subindex, uint8 value)
{
    retVal = ec_SDOwrite(slaveIdx, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTSAFE);
    return retVal;
}
int CPdoMapping::SDO_write16(uint16 slaveIdx, uint16 index, uint8 subindex, uint16 value)
{
    retVal = ec_SDOwrite(slaveIdx, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTSAFE);
    return retVal;
}
int CPdoMapping::SDO_write32(uint16 slaveIdx, uint16 index, uint8 subindex, uint32 value)
{
    retVal = ec_SDOwrite(slaveIdx, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTSAFE);
    return retVal;
}

/** Map EPOS4 motor drive PDOs.
 *
 * @param[in] slaveIdx    = Slave index.
 * @return 1 on success, 0 on failure.
 */
int CPdoMapping::mapMotorPDOs(uint16 slaveIdx)
{
    wkc = 0;

    /* <0x1600> Receive PDO Mapping 1 */
    wkc += SDO_write8(slaveIdx, 0x1600, 0x00, 0);             /* clear the PDO first */
    wkc += SDO_write32(slaveIdx, 0x1600, 0x01, 0x60400010);   // 0x60400010 : control_word               UInt16
    wkc += SDO_write32(slaveIdx, 0x1600, 0x02, 0x60600008);   // 0x60600008 : mode_of_operation          Int8
    wkc += SDO_write32(slaveIdx, 0x1600, 0x03, 0x60710010);   // 0x60710010 : target_torque              Int16
    wkc += SDO_write32(slaveIdx, 0x1600, 0x05, 0x607a0020);   // 0x607a0020 : target_position            Int32
    wkc += SDO_write32(slaveIdx, 0x1600, 0x04, 0x60ff0020);   // 0x60ff0020 : target_velocity            Int32
    wkc += SDO_write32(slaveIdx, 0x1600, 0x06, 0x60810020);   // 0x60810020 : profile_velocity           Int32
    wkc += SDO_write8(slaveIdx, 0x1600, 0x00, 6);             /* set number of objects mapped by PDO */

    wkc += SDO_write8(slaveIdx, 0x1C12, 0x00, 0);             /* clear SM2 (slave RxPDOs) */
    wkc += SDO_write16(slaveIdx, 0x1C12, 0x01, 0x1600);       /* user-PDO */
    wkc += SDO_write8(slaveIdx, 0x1C12, 0x00, 1);             /* set # of mapped PDOs */


    /* <0x1A00> Transmit PDO Mapping 1 */
    wkc += SDO_write8(slaveIdx, 0x1A00, 0x00, 0);             /* clear the PDO first */
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x01, 0x60410010);   // 0x60410010 : status_word                UInt16
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x02, 0x60610008);   // 0x60610008 : mode_of_operation_display  Int8
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x03, 0x60640020);   // 0x60640020 : position_actual_value      Int32
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x04, 0x606C0020);   // 0x606C0020 : velocity_actual_value      Int32
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x05, 0x60770010);   // 0x60770010 : torque_actual_value        Int16
    wkc += SDO_write32(slaveIdx, 0x1A00, 0x06, 0x30D10220);   // 0x30D10220 : current_actual_value       Int32
    wkc += SDO_write8(slaveIdx, 0x1A00, 0x00, 6);             /* set number of objects mapped by PDO */

    wkc += SDO_write8(slaveIdx, 0x1C13, 0x00, 0);             /* clear SM3 (slave TxPDOs) */
    wkc += SDO_write16(slaveIdx, 0x1C13, 0x01, 0x1A00);       /* user-PDO */
    wkc += SDO_write8(slaveIdx, 0x1C13, 0x00, 1);             /* set # of mapped PDOs */

    if (wkc != 22)
    {
        return 0;
    }

    printf("EPOS4 slave %d set\n", slaveIdx);
    return 1;
}

/** Initialize motor parameters via SDO
 *
 * @param slaveIdx      = Slave index.
 * @return 0.
 */
int CPdoMapping::initMotor(uint16 slaveIdx)
{
    printf("Motor drive %d init\n", slaveIdx);

    // /* Motor params */
    // SDO_write32(slaveIdx, 0x2383, 12, 25456);   /* motor torque constant */
    // SDO_write32(slaveIdx, 0x2383, 13, 650000);  /* motor peak torque */
    // SDO_write32(slaveIdx, 0x2383, 14, 20000);   /* motor continuous torque */
    // SDO_write32(slaveIdx, 0x6076, 0, 200);      /* motor rated torque */

    // /* Loop gains */
    // SDO_write16(slaveIdx, 0x2382, 1, 0);        /* position loop gain (Pp) */
    // SDO_write16(slaveIdx, 0x2381, 1, 0);        /* velocity loop gain (Vp) */

    // /* Motor limits */
    // SDO_write16(slaveIdx, 0x2110, 0, 1400);     /* peak current limit */
    // SDO_write16(slaveIdx, 0x2111, 0, 700);      /* continuous current limit (units of 0.01A) */

    return 0;
}
