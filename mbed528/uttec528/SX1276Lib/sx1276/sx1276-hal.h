/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C) 2014 Semtech

Description: -

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainers: Miguel Luis, Gregory Cristian and Nicolas Huguenin
*/
#ifndef __SX1276_HAL_H__
#define __SX1276_HAL_H__
#include "sx1276.h"

/*!
 * \brief Radio hardware registers initialization definition
 *
 * \remark Can be automatically generated by the SX1276 GUI (not yet implemented)
 */
#define RADIO_INIT_REGISTERS_VALUE                \
{                                                 \
    { MODEM_FSK , REG_LNA                , 0x23 },\
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
    { MODEM_FSK , REG_OSC                , 0x07 },\
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
}                                                 \

/*! 
 * Actual implementation of a SX1276 radio, includes some modifications to make it compatible with the MB1 LAS board
 */
class SX1276MB1xAS : public SX1276
{
protected:
    /*!
     * Antenna switch GPIO pins objects
     */
    DigitalInOut antSwitch;
    
    DigitalIn fake;
    
private:
    static const RadioRegisters_t RadioRegsInit[];
    
public:
    SX1276MB1xAS( RadioEvents_t *events,
            PinName mosi, PinName miso, PinName sclk, PinName nss, PinName reset,
            PinName dio0, PinName dio1, PinName dio2, PinName dio3, PinName dio4, PinName dio5,
            PinName antSwitch ); 
    
    SX1276MB1xAS( RadioEvents_t *events );
    
    virtual ~SX1276MB1xAS( ) { };
    
    protected:
    /*!
     * @brief Initializes the radio I/Os pins interface
     */
    virtual void IoInit( void );
    
    /*!
     *  @brief Initializes the radio registers
     */
    virtual void RadioRegistersInit( );
    
    /*!
     * @brief Initializes the radio SPI
     */
    virtual void SpiInit( void );
    
    /*!
     * @brief Initializes DIO IRQ handlers
     *
     * @param [IN] irqHandlers Array containing the IRQ callback functions
     */
    virtual void IoIrqInit( DioIrqHandler *irqHandlers );

    /*!
     * @brief De-initializes the radio I/Os pins interface. 
     *
     * \remark Useful when going in MCU lowpower modes
     */
    virtual void IoDeInit( void );

    /*!
     * @brief Gets the board PA selection configuration
     *
     * @param [IN] channel Channel frequency in Hz
     * @retval PaSelect RegPaConfig PaSelect value
     */
    virtual uint8_t GetPaSelect( uint32_t channel );

    /*!
     * @brief Set the RF Switch I/Os pins in Low Power mode
     *
     * @param [IN] status enable or disable
     */
    virtual void SetAntSwLowPower( bool status );

    /*!
     * @brief Initializes the RF Switch I/Os pins interface
     */
    virtual void AntSwInit( void );

    /*!
     * @brief De-initializes the RF Switch I/Os pins interface 
     *
     * \remark Needed to decrease the power consumption in MCU lowpower modes
     */
    virtual void AntSwDeInit( void );

    /*!
     * @brief Controls the antena switch if necessary.
     *
     * \remark see errata note
     *
     * @param [IN] rxTx [1: Tx, 0: Rx]
     */
    virtual void SetAntSw( uint8_t rxTx );
    
    public:
    /*!
     * @brief Detect the board connected by reading the value of the antenna switch pin
     */
    virtual uint8_t DetectBoardType( void );    
    
    /*!
     * @brief Checks if the given RF frequency is supported by the hardware
     *
     * @param [IN] frequency RF frequency to be checked
     * @retval isSupported [true: supported, false: unsupported]
     */
    virtual bool CheckRfFrequency( uint32_t frequency );
    
        /*!
     * @brief Writes the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @param [IN]: data New register value
     */
    virtual void Write ( uint8_t addr, uint8_t data ) ;
    
    /*!
     * @brief Reads the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @retval data Register value
     */
    virtual uint8_t Read ( uint8_t addr ) ;
    
    /*!
     * @brief Writes multiple radio registers starting at address
     *
     * @param [IN] addr   First Radio register address
     * @param [IN] buffer Buffer containing the new register's values
     * @param [IN] size   Number of registers to be written
     */
    virtual void Write( uint8_t addr, uint8_t *buffer, uint8_t size ) ;
    
    /*!
     * @brief Reads multiple radio registers starting at address
     *
     * @param [IN] addr First Radio register address
     * @param [OUT] buffer Buffer where to copy the registers data
     * @param [IN] size Number of registers to be read
     */
    virtual void Read ( uint8_t addr, uint8_t *buffer, uint8_t size ) ;
    
    /*!
     * @brief Writes the buffer contents to the SX1276 FIFO
     *
     * @param [IN] buffer Buffer containing data to be put on the FIFO.
     * @param [IN] size Number of bytes to be written to the FIFO
     */
    virtual void WriteFifo( uint8_t *buffer, uint8_t size ) ;

    /*!
     * @brief Reads the contents of the SX1276 FIFO
     *
     * @param [OUT] buffer Buffer where to copy the FIFO read data.
     * @param [IN] size Number of bytes to be read from the FIFO
     */
    virtual void ReadFifo( uint8_t *buffer, uint8_t size ) ;
    
    /*!
     * @brief Reset the SX1276
     */
    virtual void Reset( void );
};

#endif // __SX1276_HAL_H__
