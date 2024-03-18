# PHP Version Manager For Ubuntu

## Introduction
This is a CLI tool to manage PHP versions on Ubuntu. It's a C program that can be used to install, uninstall, and switch between PHP versions. It's a simple tool that can be used to manage PHP versions on Ubuntu.

## Installation
To install the tool, you can run the following commands:

```sh
TO BE UPDATED
```

## Usage
To use the tool, you can run the following commands:

```sh
$ nvm list
Available PHP versions:
    7.3
    7.4
  * 8.2
  
$ nvm install 8.1
PHP version 8.1 is installed.
You can use it by running 'pvm use 8.1'.

$ nvm use 8.1
Now using PHP version 8.1.

$ php -v
PHP 8.1.27 (cli) (built: Dec 21 2023 20:19:54) (NTS)
Copyright (c) The PHP Group
Zend Engine v4.1.27, Copyright (c) Zend Technologies
    with Zend OPcache v8.1.27, Copyright (c), by Zend Technologies

$ nvm uninstall 8.1
PHP version 8.1 is uninstalled.
```

## Alternatives
If you are looking for an alternative to this tool,
 - [fatihozpolat/pvm-laragon](https://github.com/fatihozpolat/pvm-laragon)
 - [fatihozpolat/pvm-windows](https://github.com/fatihozpolat/pvm-windows)

Do you have an alternative? Please add it here and create a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
