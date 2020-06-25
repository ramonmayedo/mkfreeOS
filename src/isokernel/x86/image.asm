;Formato ISO el Torito
sizeSector equ 2048
logicImageLBA equ 27
;Sector 0...15 : inicializados en 0
TIMES (sizeSector*16) - ($-$$ ) DB 0
;Sector 16: Primary Volume Descriptor
db  1                       ;0    Descriptor Type  	(Primary Volume Descriptor = 1)
db  'CD001'                 ;1-5 (Standard Identifier Type)
db  1                       ;6   (Descriptor Version)
db  0                       ;7   (Unused)
sysId:         
db  'mkfree'                ;8-39:(System Identifier)
TIMES 32 - ($-sysId) DB ' '
volId:
db   'mkfree.iso'           ;40-71:(Volume Identifier)
TIMES 32 - ($-volId) DB ' '
TIMES 8 DB 0x0              ;72-79:  Unused Field
dd    0x00000080,0x80000000 ;80-87:  Volume Space Size
TIMES 32 DB 0x0             ;88-119: Unused Field 
dw    0x0001,0x0100         ;120-123: Volume Set Size 
dw    0x0001,0x0100         ;124-127: Volume Sequence Number
dw    0x0800,0x0008         ;128-131: Logical Block Size 
dd    0,0                   ;132-139: Path Table Size 
dd    0                     ;140-143: Location of Type-L Path Table 
dd    0                     ;144-147: Location of the Optional Type-L Path Table 
dd    0                     ;148-151: Location of Type-M Path Table 
dd    0                     ;152-155: Location of Optional Type-M Path Table
TIMES 34 DB 0               ;156-189: Directory entry for the root directory
corp:
db 'mkcorp'
TIMES 128 -($-corp) db ' '
;TIMES 128 DB ' '            ;190-317: Volume Set Identifier 
TIMES 128 DB ' '            ;318-445: Publisher Identifier 
TIMES 128 DB ' '            ;446-573: Data Preparer Identifier 
TIMES 128 DB ' '            ;574-701: Application Identifier
TIMES 38 DB ' '             ;702-739: Copyright File Identifier
TIMES 36 DB ' '             ;740-775: Abstract File Identifier
TIMES 37 DB ' '             ;776-812: Bibliographic File Identifier 
TIMES 17 DB ' '             ;813-829: Volume Creation Date and Time
TIMES 17 DB ' '             ;830-846: Volume Modification Date and Time
TIMES 17 DB ' '             ;847-863: Volume Expiration Date and Time 
TIMES 17 DB ' '             ;864-880: Volume Effective Date and Time
db 0x1                      ;881: File Structure Version 
db 0x0                      ;882: Unused
TIMES 512 DB 0x0            ;883-1394  :Application Used 
TIMES 653 DB 0x0            ;1395-2047 : Reserved 

;Sector  17: Boot Record Volume Descriptor
db 0                         ;0    :   Volume Descriptor Type (Boot Record  =0 )
db 'CD001'                   ;1-5  :  (ISO 9660 Specifier)
db 1                         ;6    : (Descriptor Version)
bSysID:
db 'EL TORITO SPECIFICATION' ;7-38 :System Identifier
TIMES 32 -($-bSysID) db 0x0
TIMES 32 db 0x0              ;39-70   :Boot Identifier
dd   19                      ;71-73   :It records at bytes 71 to 74 as little-endian 32-bit number the block address of the El Torito Boot Catalog.
TIMES 1973 db 0x0            ;74-2048 :Custom - used by the boot system. 


;Sector  18: Volume Descriptor Terminator
db   0xff                     ;0      : Descriptor Type (Volume Descriptor Set Terminator 0xFF)
db   'CD001'                  ;1-5    : Standard Identifier Type
db  1                         ;6      : Descriptor Version
TIMES 2041 DB 0x0             ;7-2048 :	Data  Depends on the volume descriptor type. 

;Sector 19: Catalog from Torito Expecification
;Section Entry Extension
;Validation Entry
db   0x1                     ;0    :Header ID, must be 01
db   0x0                     ;1    :Platform ID 0 = 80x86 1=Power PC 2=Mac
dw   0x0                     ;2-3  :Reserved, must be 0
cdId:
db  0x0                 ;4-27 :ID string. This is intended to identify the manufacturer/developer of the CD-ROM. 
TIMES 24 - ($-cdId) db 0x0   ;
dw    0x55AA                 ;28-29:Checksum Word. This sum of all the words in this record should be 0.
db    0x55                   ;30   :Key byte, must be 55. This value is included in the checksum.
db    0xAA                   ;31   :Key byte, must be AA. This value is included in the checksum.
;Initial/Default Entry
db    0x88                   ;0     :Boot Indicator. 88 = Bootable, 00 = Not Bootable
db    0x0                    ;1     :Boot media type 0 No Emulation
dw    0x0                    ;2-3   :Load Segment. This is the load segment for the initial boot image. If this value is 0 the system will use the traditional segment of 7C0
db    0x0                    ;4     :System Type. This must be a copy of byte 5 (System Type) from the Partition Table found in the boot image.
db    0x0                    ;5     :Unused, must be 0
dw    20                     ;6-7    :Sector Count. This is the number of virtual/emulated sectors the system will store at Load Segment during the initial boot procedure.
dd    logicImageLBA         ;8-11  :Load RBA. This is the start address of the virtual disk. CD’s use Relative/Logical block addressing.
TIMES 20 db 0x0             ;12-32 :Unused, must be 0.

TIMES (sizeSector*logicImageLBA) - ($-$$ ) DB 0

INCBIN "x86/bootloader.o"


TIMES (sizeSector*70) - ($-$$ ) DB 0