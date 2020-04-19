# Password Wallet
[![Build Status](https://travis-ci.com/ThomasChevalier/passwordWallet.svg?token=WzsuqMps55SajLjgcmJx&branch=master)](https://travis-ci.com/ThomasChevalier/passwordWallet)

Password Wallet is a physical password manager, working on every platforms by emulating an HID keyboard to interface with the computer.

![](Doc/overview.gif)

## Description

Password Wallet is a USB device that can store credentials (passwords and user names) securely. The manipulation of the stored data as well as the various actions of restitution are available directly on the device which makes it an autonomous object, in the sense that a software is not necessary to use it. The device is designed to be reliable and robust, taking into account the sensitivity of the stored information. This project is what we could call a hardware password manager.
This object differs from software password managers by its portability: it is available on any operating system without prior installation, which facilitates implementation on computers with restricted access. Whereas most password managers offer to save data online to facilitate its transmission, and therefore expose this data to hackers, Password Wallet only stores data in its internal memory, so the identifiers are only gathered in one place, which ensures their security. In addition, its small size, approximately the size of a credit card, allows it to be stored in a pocket or bag.

## Features

* Credentials are AES-128 encrypted
* Unlock it with your RFID card
* Backup mechanism available if your card is lost
* Hardware random number generator based on clock jitter
* Store more than 800 credentials on the same device
* Sort credentials by their name, by date or by number of use
* Add, edit and delete credentials directly with the device
* Generate strong random passwords
* Highly durable memory, using Ferroelectric RAM
* High-contrast display using OLED technology
* Low power usage, maximizing notebook batteries
* Supports Qwerty and Azerty keyboards
* User-proof system, resists to unexpected disconnections
* Desktop application available to communicate with Password Wallet for easier managment
