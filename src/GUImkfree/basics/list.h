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

#ifndef LIST_H
#define LIST_H

class Cnode {
private:
    Cnode *next, *prev;
    void  *data;
public:
    Cnode(void  *adata){data=adata;next=0;prev=0;}
    void setData(void *adata){data = adata;}
    void setNext(Cnode *anext){next=anext;}
    void setPrev(Cnode *aprev){prev=aprev;}
    void *getData(){return data;}
    Cnode* getNext(){return next;}
    Cnode* getPrev(){return prev;}
};

class Clist {
private:
    Cnode *firstNode;
    Cnode *lastNode;
    int ncount;
public:
    Clist();
    int add(void * aitem);
    void *getFirst();
    void *getLast();
    void *getItem(int aindex);
    void * operator[](int index);
    int count();
    bool isEmty();
    void setItem(int aindex, void *aitem);
    void move(int aindex1, int aindex2);
    void delet(int aindex);
    void clear();
    void insert(int aindex, void *aitem);
    int indexOf(void *aitem);
    int remove(void *aitem);
    void *removeLast();
    void *removeFirst(); 
    ~Clist();
protected:
    Cnode *getNode(int aindex);
    void addNode(void * aitem, Cnode *anode);
};

#endif

