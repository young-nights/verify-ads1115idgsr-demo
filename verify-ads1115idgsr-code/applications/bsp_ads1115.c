/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-12-01     18452       the first version
 */
#include "bsp_ads1115.h"




iicStructure_t ads1115_iic = {
        .i2c_name       = "i2c1",   /* 设备结点 */
        .i2c_addr       = ADS1115_GND_ADDR,     /* ads1115的iic地址 */
};


/**
 * @brief   ads1115初始化IIC总线
 * @param   None
 * @return  None
 */
void ads1115_device_init(void)
{
    ads1115_iic.i2c_bus = (struct rt_i2c_bus_device *)(rt_device_find(ads1115_iic.i2c_name));
    if(ads1115_iic.i2c_bus != RT_NULL){
        rt_kprintf("PRINTF:%d. ads1115_iic bus is found!\r\n",Record.kprintf_cnt++);
    }
    else {
        rt_kprintf("ads1115_iic bus can't find!\r\n");
    }
}





/**
 * @brief  向 ADS1115 的指定寄存器写入 16bit 配置数据（高字节在前）
 * @param  reg   : 寄存器地址（0x00 ~ 0x03）
 *                  - 0x00 : Conversion register（转换结果）
 *                  - 0x01 : Config register    （配置寄存器） ← 我们最常用
 *                  - 0x02 : Lo_thresh register （低阈值）
 *                  - 0x03 : Hi_thresh register （高阈值）
 * @param  data  : 要写入的 16bit 数据（ADS1115 所有寄存器都是 16bit）
 * @retval RT_EOK  : 写入成功
 *         RT_ERROR: 写入失败（I2C 通信异常）
 */
rt_err_t iic_ads1115_write_reg(uint8_t reg, uint16_t data)
{
    uint8_t buf[3];   // I2C 要发送的完整数据包：寄存器地址 + 16bit 数据
    struct rt_i2c_msg ads1115_msg;   // RT-Thread 的 I2C 消息结构体

    /* 第一步：组装要发送的数据 */
    buf[0] = reg;                    // 第1个字节：目标寄存器地址
    buf[1] = (uint8_t)(data >> 8);   // 第2个字节：数据高8位（ADS1115 是大端模式）
    buf[2] = (uint8_t)(data & 0xFF); // 第3个字节：数据低8位

    /* 第二步：填充 I2C 消息结构体 */
    ads1115_msg.addr  = ads1115_dev.i2c_addr;   // 从设备地址（7位），比如 0x48
    ads1115_msg.flags = RT_I2C_WR;              // 写操作（RT_I2C_WR = 0）
    ads1115_msg.buf   = buf;                    // 要发送的数据缓冲区指针
    ads1115_msg.len   = 3;                      // 总共发送 3 个字节（1字节寄存器地址 + 2字节数据）

    if (rt_i2c_transfer(ads1115_iic.i2c_bus, &msg, 1) == 1)
        return RT_EOK;
    else
        return RT_ERROR;
}




/**
 * @brief  从 ADS1115 的指定寄存器读取 16bit 数据（高字节在前）
 * @param  reg       : 要读取的寄存器地址
 *                     - 0x00 : Conversion 寄存器（转换结果，最常用！）
 *                     - 0x01 : Config 寄存器（当前配置）
 *                     - 0x02 : Lo_thresh 寄存器
 *                     - 0x03 : Hi_thresh 寄存器
 * @param  i2c_dat   : 指针，用于返回读取到的 16bit 数据
 * @retval RT_EOK    : 读取成功
 *         RT_ERROR  : 读取失败（I2C 总线错误、从机无应答等）
 */
rt_err_t iic_ads1115_read_reg(rt_uint8_t reg, rt_uint16_t* i2c_dat)
{

    uint8_t tmp = reg;
    uint8_t buf[2];
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = ads1115_iic.i2c_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &tmp;
    msgs[0].len   = 1;

    msgs[1].addr  = ads1115_iic.i2c_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = 2;

    if (rt_i2c_transfer(ads1115_iic.i2c_bus, msgs, 2) == 2) {
        *i2c_dat = (buf[0] << 8) | buf[1];
        return RT_EOK;
    }
    return RT_ERROR;
}












