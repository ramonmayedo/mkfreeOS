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

#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#define ARCHX86_GENERAL_FAULT 0x1
#define ARCHX86_PAGE_FAULT 0x2
#define ARCHX86_MEMORY_CORRUPT 0x3

class Carchitecture {
public:
    Carchitecture();
    void changeToDirectoryKernel();
    void changeToDirectoryCurrentProcess();
    void setDirectory(unsigned int adirectorty);
    void deseableInterruption();
    void enabledInterruption();
    void reset();
    void shutDown();
    void kernelStopScreen(int stopMode);
private:

};

#endif

