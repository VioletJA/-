#include "ds3231.h"
#include "i2c.h"


uint8_t I2C_DS3231_ByteWrite(uint8_t WriteAddr , uint8_t date)
{
	I2CStart();
	//printf("I2C_DS3231_ByteWrite start!");
	I2CSendByte(DS3231_ADDRESS_Write);
	//printf("write finish,wait ask!");
    if(I2CWaitAck())
		return 1;
	//printf("add ask ok!");
	I2CSendByte(WriteAddr);
	if(I2CWaitAck())
		return 2;
	//printf("date ask ok!");
	I2CSendByte(date);
	if(I2CWaitAck())
		return 3;
	I2CStop();
	return 0;
}


uint8_t I2C_DS3231_ByteRead(uint8_t ReadAddr,uint8_t* Receive)
{
	unsigned char data = 0;
	I2CStart();													
	I2CSendByte(DS3231_ADDRESS_Write); 	
    if(I2CWaitAck())
		return 1;
	I2CSendByte(ReadAddr);							
	if(I2CWaitAck())
		return 2;
	I2CStart();													
	I2CSendByte(DS3231_ADDRESS_Read);		
	if(I2CWaitAck())
		return 3;
	data = I2CReceiveByte();	 						
	I2CSendNotAck();
	*Receive = data;											
	I2CStop();
	return 0;
}


void DS3231_setDate(uint8_t year,uint8_t mon,uint8_t day)
{
	uint8_t temp_H , temp_L;
	temp_L = year%10;
	temp_H = year/10;
	year = (temp_H << 4) + temp_L;
	//printf("start DS3231 SETDATE \r\n");
	I2C_DS3231_ByteWrite(DS3231_YEAR_REG,year); //set year
	//printf("set year error\r\n");
	//printf("year ok \r\n");
	temp_L = mon%10;
	temp_H = mon/10;
	mon = (temp_H << 4) + temp_L;
	I2C_DS3231_ByteWrite(DS3231_MONTH_REG,mon); //set mon
	//printf("month ok \r\n");
	temp_L = day%10;
	temp_H = day/10;
	day = (temp_H << 4) + temp_L;
	I2C_DS3231_ByteWrite(DS3231_MDAY_REG,day); //set day
}


void DS3231_setTime(uint8_t hour , uint8_t min , uint8_t sec)
{
	uint8_t temp_H , temp_L;
	temp_L = hour%10;
	temp_H = hour/10;
	hour = (temp_H << 4) + temp_L;
	//printf("start DS3231 SETTIME \r\n");
	I2C_DS3231_ByteWrite(DS3231_HOUR_REG,hour); //set hour
    //printf("set time error \r\n");
	//printf("HOUR OK \r\n");
	temp_L = min%10;
	temp_H = min/10;
	min = (temp_H << 4) + temp_L;
	I2C_DS3231_ByteWrite(DS3231_MIN_REG,min); //SET min
		//printf("set time error \r\n");
	//printf("MIN OK \r\n");
	temp_L = sec%10;
	temp_H = sec/10;
	sec = (temp_H << 4) + temp_L;
	I2C_DS3231_ByteWrite(DS3231_SEC_REG,sec);
		//printf("set time error \r\n");//SET sec
	//printf("ALL OK \r\n");
}

static uint8_t bcdToDec(uint8_t byte)
{
	uint8_t temp_H , temp_L;
	temp_L = byte & 0x0f;
	temp_H = (byte & 0xf0) >> 4;
	return ( temp_H * 10 )+ temp_L;
}


void DS3231_getTime(DateTime* ans)
{
	uint8_t receive = 0;
	I2C_DS3231_ByteRead(DS3231_HOUR_REG,&receive);
	ans->hour = bcdToDec(receive);
	I2C_DS3231_ByteRead(DS3231_MIN_REG,&receive);
	ans->minute = bcdToDec(receive);
	I2C_DS3231_ByteRead(DS3231_SEC_REG,&receive);
	ans->second = bcdToDec(receive);
}

void DS3231_getDate(DateTime* ans)
{
	uint8_t receive = 0;
	I2C_DS3231_ByteRead(DS3231_YEAR_REG,&receive);
	ans->year = bcdToDec(receive) + 2000;
	I2C_DS3231_ByteRead(DS3231_MONTH_REG,&receive);
	ans->month = bcdToDec(receive);
	I2C_DS3231_ByteRead(DS3231_MDAY_REG,&receive);
	ans->dayofmonth = bcdToDec(receive);
}

