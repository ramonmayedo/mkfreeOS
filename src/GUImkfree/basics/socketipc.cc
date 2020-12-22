/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "socketipc.h"

SsocketIPC::SsocketIPC(int apid) {
    pid = apid;
}

void SsocketIPC::readData(void* data, int offset, int size) {
    readBufferIPC(pid, (char*) data, offset, size);
}

void SsocketIPC::writeData(void* data, int offset, int size) {
    writeBufferIPC(pid, (char*) data, offset, size);
}

void SsocketIPC::command(int command, int par1, int par2) {
    sendCommand(command, pid, par1, par2);
}

SsocketIPC::~SsocketIPC() {
}