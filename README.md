# elf-packer
elf64 packer application

Produces a copy of an elf64 with an encrypted text section. Self decrypts when it runs.

Intended for use on elf64 format, use on other formats is undefined at this time.

## Install
`make`

To add binary for testing in the test folder:

`make test`

## Usage

`packer [elf64 filepath]`
