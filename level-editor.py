#!/usr/bin/env python

__doc__="""controls:

to fill a rectangle of squares, (r/l)click the two corners of the rectangle
w     - add a row (top)
s     - add a row (bottom)
a     - add a column (left)
d     - add a column (right)
k     - remove a row (top) - k
i     - remove a row (bottom) - i
l     - remove a column (left) - l
j     - remove a column (right) - j
enter - output space separated file"""

import pygame
import sys
import random

TILE_WIDTH = 16

class Camera:
    truex, truey, truew, trueh = 0, 0, 0, 0
    locx, locy, locw, loch = 0, 0, 0, 0
    up, down, left, right = False, False, False, False
    zoomout, zoomin = False, False

    def __init__(self, lvlw, lvlh, w, h):
        self.locx, self.locy = float(lvlw)/2., float(lvlh)/2.
        self.locw, self.loch = float(w), float(h)
        self.truex = self.locx - self.locw / 2.
        self.truey = self.locy - self.loch / 2.
        self.truew, self.trueh = self.locw, self.loch

    def handle_event(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                self.left = True
            elif event.key == pygame.K_RIGHT:
                self.right = True
            elif event.key == pygame.K_DOWN:
                self.down = True
            elif event.key == pygame.K_UP:
                self.up = True
            elif event.key == pygame.K_z:
                self.zoomout = True
            elif event.key == pygame.K_x:
                self.zoomin = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT:
                self.left = False
            elif event.key == pygame.K_RIGHT:
                self.right = False
            elif event.key == pygame.K_DOWN:
                self.down = False
            elif event.key == pygame.K_UP:
                self.up = False
            elif event.key == pygame.K_z:
                self.zoomout = False
            elif event.key == pygame.K_x:
                self.zoomin = False

    def update(self, screen, w, h):
        scr_w, scr_h = screen.get_width(), screen.get_height()
        if self.up:
            self.locy -= 30.
        if self.down:
            self.locy += 30.
        if self.right:
            self.locx += 30.
        if self.left:
            self.locx -= 30.
        if self.zoomout:
            if self.locw > TILE_WIDTH*4:
                self.locw /= 1.1
                self.loch = self.locw * (float(scr_h) / float(scr_w))
        if self.zoomin:
            self.locw *= 1.1
            self.loch = self.locw * (float(scr_h) / float(scr_w))
        self.truex = self.locx - self.locw / 2.
        self.truey = self.locy - self.loch / 2.
        self.truew, self.trueh = self.locw, self.loch

    def get_rect(self):
        return self.truex, self.truey, self.truew, self.trueh

    def move_up(self):
        self.locy -= TILE_WIDTH

    def move_down(self):
        self.locy += TILE_WIDTH

    def move_left(self):
        self.locx -= TILE_WIDTH

    def move_right(self):
        self.locx += TILE_WIDTH

class Border:
    def __init__(self, w, h):
        self.x, self.y, self.w, self.h = 0., 0., float(w)*TILE_WIDTH, float(h)*TILE_WIDTH

    def draw(self, screen, cam):
        camx, camy, camw, camh = cam
        x1 = (self.x - camx) / (camw / float(screen.get_width()))
        y1 = (self.y - camy) / (camh / float(screen.get_height()))
        x2 = x1 + self.w * (screen.get_width() / camw)
        y2 = y1 + self.h * (screen.get_height() / camh)
        pointlist = [[x1, y1], [x2, y1], [x2, y2], [x1, y2]]
        pygame.draw.lines(screen, (255,0,0),True, pointlist, 3)

    def update(self, w, h):
        self.w, self.h = float(w)*TILE_WIDTH, float(h)*TILE_WIDTH

class Gridlines:
    def __init__(self, w, h):
        self.w, self.h = w, h

    def draw(self, screen, cam):
        camx, camy, camw, camh = cam
        scr_w, scr_h = screen.get_width(), screen.get_height()
        # vertical lines
        for i in range(1,self.w):
            x = (i*TILE_WIDTH - camx) / (camw / float(scr_w))
            y1 = (-camy) / (camh / float(scr_h))
            y2 = (h*TILE_WIDTH - camy) / (camh / float(scr_h))
            pygame.draw.line(screen, (0,200,0), [x, y1], [x, y2])
        # horizontal lines
        for j in range(1,self.h):
            y = (j*TILE_WIDTH - camy) / (camh / float(scr_h))
            x1 = (-camx) / (camw / float(scr_w))
            x2 = (w*TILE_WIDTH - camx) / (camw / float(scr_w))
            pygame.draw.line(screen, (0,200,0), [x1, y], [x2, y])

    def update(self, w, h):
        self.w, self.h = w, h

class Tileset:
    array = []
    obj_array = []
    rect = 0
    x1, y1 = 0, 0

    def __init__(self, w, h, array=None, obj_array=None):
        if not array and not obj_array:
            self.array = [[False]*w for _ in range(h)]
        else:
            self.array = array
            self.obj_array = obj_array

    def draw(self, screen, cam):
        camx, camy, camw, camh = cam
        scr_w, scr_h = screen.get_width(), screen.get_height()
        for i in range(len(self.array)):
            for j in range(len(self.array[0])):
                if self.array[i][j] == 1:
                    x1 = (j*TILE_WIDTH - camx) / (float(camw) / float(scr_w))
                    x2 = ((j+1)*TILE_WIDTH - camx) / (float(camw) / float(scr_w)) + 1
                    y1 = (i*TILE_WIDTH - camy) / (float(camh) / float(scr_h))
                    y2 = ((i+1)*TILE_WIDTH - camy) / (float(camh) / float(scr_h)) + 1
                    rect = (x1, y1, x2-x1, y2-y1)
                    pygame.draw.rect(screen, (0,0,0), rect)
                elif self.array[i][j] > 1:
                    x1 = (j*TILE_WIDTH - camx) / (float(camw) / float(scr_w))
                    x2 = ((j+1)*TILE_WIDTH - camx) / (float(camw) / float(scr_w)) + 1
                    y1 = (i*TILE_WIDTH - camy) / (float(camh) / float(scr_h))
                    y2 = ((i+1)*TILE_WIDTH - camy) / (float(camh) / float(scr_h)) + 1
                    rect = (x1, y1, x2-x1, y2-y1)
                    if self.array[i][j] == 2:
                        pygame.draw.rect(screen, (0,0,255), rect)
                    else:
                        pygame.draw.rect(screen, (255,255,0), rect)
        for i in range(len(self.obj_array)):
            x1 = (self.obj_array[i][1]*TILE_WIDTH - camx + 1) / (float(camw) / float(scr_w))
            x2 = ((self.obj_array[i][1]+1)*TILE_WIDTH - camx - 1) / (float(camw) / float(scr_w))
            y1 = (self.obj_array[i][2]*TILE_WIDTH - camy + 1) / (float(camh) / float(scr_h))
            y2 = ((self.obj_array[i][2]+1)*TILE_WIDTH - camy - 1) / (float(camh) / float(scr_h))
            rect = (x1, y1, x2-x1, y2-y1)
            color = (255,255,255)
            if self.obj_array[i][3]:
                color = (0,0,0)
            if self.obj_array[i][0] == "char":
                pygame.draw.ellipse(screen, color, rect)
            elif self.obj_array[i][0] == "level_end":
                pygame.draw.rect(screen, color, rect)
            elif self.obj_array[i][0] == "button":
                rect = (x1, y1, (x2-x1)/2, (y2-y1)/2)
                pygame.draw.ellipse(screen, color, rect)
            elif self.obj_array[i][0] == "spring":
                rect = (x1+(x2-x1)/2, y1+(y2-y1)/2, (x2-x1)/2, (y2-y1)/2)
                pygame.draw.ellipse(screen, color, rect)

    def handle_event(self, event, screen, cam, placing):
        scr_w, scr_h = screen.get_width(), screen.get_height()
        camx, camy, camw, camh = cam
        if event.type == pygame.MOUSEBUTTONDOWN:
            # placing tiles
            if placing == 0:
                if not self.rect:
                    mousex, mousey = pygame.mouse.get_pos()
                    x1 = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                    y1 = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                    if x1 >= len(self.array[0]) or y1 >= len(self.array) or x1 < 0 or y1 < 0:
                        return
                    self.x1 = x1
                    self.y1 = y1
                    self.rect = event.button
                    if event.button == 1:
                        self.rect = 1
                        self.array[self.y1][self.x1] = 2
                    elif event.button == 3:
                        self.rect = 3
                        self.array[self.y1][self.x1] = 3
                else:
                    mousex, mousey = pygame.mouse.get_pos()
                    mx = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                    my = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                    if mx >= len(self.array[0]) or my >= len(self.array) or mx < 0 or my < 0:
                        return
                    if self.rect == 1:
                        self.fill_rect(True, mx, my)
                    elif self.rect == 3:
                        self.fill_rect(False, mx, my)
            # placing chars
            elif placing == 1:
                mousex, mousey = pygame.mouse.get_pos()
                x = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                y = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                if x >= len(self.array[0]) or y >= len(self.array) or x < 0 or y < 0:
                    return
                for i in range(len(self.obj_array)):
                    if self.obj_array[i][0] == "char" and self.obj_array[i][3] == (event.button == 1):
                        self.obj_array.pop(i)
                        break
                self.obj_array.append(["char", x, y, (event.button == 1)])
            # placing level ends
            elif placing == 2:
                mousex, mousey = pygame.mouse.get_pos()
                x = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                y = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                if x >= len(self.array[0]) or y >= len(self.array) or x < 0 or y < 0:
                    return
                for i in range(len(self.obj_array)):
                    if self.obj_array[i][0] == "level_end" and self.obj_array[i][3] == (event.button == 1):
                        self.obj_array.pop(i)
                        break
                self.obj_array.append(["level_end", x, y, (event.button == 1)])
            # placing buttons
            elif placing == 3:
                mousex, mousey = pygame.mouse.get_pos()
                x = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                y = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                if x >= len(self.array[0]) or y >= len(self.array) or x < 0 or y < 0:
                    return
                for i in range(len(self.obj_array)):
                    if self.obj_array[i][0] == "button" and self.obj_array[i][3] == (event.button == 1):
                        self.obj_array.pop(i)
                        break
                self.obj_array.append(["button", x, y, (event.button == 1)])
            # springs
            elif placing == 4:
                mousex, mousey = pygame.mouse.get_pos()
                x = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                y = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                if x >= len(self.array[0]) or y >= len(self.array) or x < 0 or y < 0:
                    return
                for i in range(len(self.obj_array)):
                    if self.obj_array[i][0] == "spring" and self.obj_array[i][3] == (event.button == 1):
                        self.obj_array.pop(i)
                        break
                self.obj_array.append(["spring", x, y, (event.button == 1)])
            # deleting stuff
            elif placing == -1:
                mousex, mousey = pygame.mouse.get_pos()
                x = int((mousex * (float(camw) / float(scr_w)) + camx) / TILE_WIDTH)
                y = int((mousey * (float(camh) / float(scr_h)) + camy) / TILE_WIDTH)
                for i in range(len(self.obj_array)):
                    if self.obj_array[i][1] == x and self.obj_array[i][2] == y:
                        self.obj_array.pop(i)

    def fill_rect(self, black, mx, my):
        x1, x2 = min(self.x1, mx), max(self.x1, mx)
        y1, y2 = min(self.y1, my), max(self.y1, my)
        for i in range(y1, y2+1):
            for j in range(x1, x2+1):
                self.array[i][j] = black
        self.rect = 0

    def add_row_bottom(self):
        w = len(self.array[0])
        self.array += [[False]*w]

    def remove_row_bottom(self):
        self.array = self.array[:-1]

    def add_row_top(self):
        w = len(self.array[0])
        self.array.insert(0, [False]*w)

    def remove_row_top(self):
        self.array = self.array[1:]

    def add_col_right(self):
        for i in range(len(self.array)):
            self.array[i] += [False]

    def remove_col_right(self):
        for i in range(len(self.array)):
            self.array[i] = self.array[i][:-1]

    def add_col_left(self):
        for i in range(len(self.array)):
            self.array[i].insert(0, False)

    def remove_col_left(self):
        for i in range(len(self.array)):
            self.array[i] = self.array[i][1:]

    def get_array(self):
        return self.array

    def get_obj_array(self):
        return self.obj_array

def collide_pt(rect, pt):
    rx, ry, rw, rh = rect
    px, py = pt
    return px > rx and px < rx+rw and py > ry and py < ry+rh

def get_str(screen, prompt):
    myfont = pygame.font.SysFont('Times New Roman', 30)
    res = ""
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    sys.exit()
                elif event.key == pygame.K_0:
                    res += '0'
                elif event.key == pygame.K_1:
                    res += '1'
                elif event.key == pygame.K_2:
                    res += '2'
                elif event.key == pygame.K_3:
                    res += '3'
                elif event.key == pygame.K_4:
                    res += '4'
                elif event.key == pygame.K_5:
                    res += '5'
                elif event.key == pygame.K_6:
                    res += '6'
                elif event.key == pygame.K_7:
                    res += '7'
                elif event.key == pygame.K_8:
                    res += '8'
                elif event.key == pygame.K_9:
                    res += '9'
                elif event.key == pygame.K_RETURN:
                    return res
                elif event.key == pygame.K_BACKSPACE:
                    res = res[:-1]
        textsurface = myfont.render(prompt+res, False, (0, 0, 0))
        tex_w, tex_h = myfont.size(prompt+res)
        scr_w, scr_h = screen.get_width(), screen.get_height()

        screen.fill(white)
        screen.blit(textsurface, (scr_w/2-tex_w/2,scr_h/2-tex_h/2))
        pygame.display.flip()

def output_arr(array):
    w = len(array[0])
    h = len(array)
    result = [['']*w for _ in range(h)]
    for i in range(h):
        for j in range(w):
            # if this is a black square
            if array[i][j]:
                if i == 0 and j == 0:
                    if array[i+1][j]:
                        if array[i][j+1]:
                            if not array[i+1][j+1]:
                                result[i][j] = '13'
                            else:
                                result[i][j] = '00'
                        else:
                            result[i][j] = '09'
                    else:
                        if array[i][j+1]:
                            result[i][j] = '06'
                        else:
                            result[i][j] = '07'
                elif i == 0 and j == w-1:
                    if array[i+1][j]:
                        if array[i][j-1]:
                            if not array[i+1][j-1]:
                                result[i][j] = '14'
                            else:
                                result[i][j] = '00'
                        else:
                            result[i][j] = '10'
                    else:
                        if array[i][j-1]:
                            result[i][j] = '06'
                        else:
                            result[i][j] = '08'
                elif i == h-1 and j == 0:
                    if array[i-1][j]:
                        if array[i][j+1]:
                            if not array[i-1][j+1]:
                                result[i][j] = '11'
                            else:
                                result[i][j] = '00'
                        else:
                            result[i][j] = '09'
                    else:
                        if array[i][j+1]:
                            result[i][j] = random.choice(['01','02','03'])
                        else:
                            result[i][j] = '04'
                elif i == h-1 and j == w-1:
                    if array[i-1][j]:
                        if array[i][j-1]:
                            if not array[i-1][j-1]:
                                result[i][j] = '12'
                            else:
                                result[i][j] = '00'
                        else:
                            result[i][j] = '10'
                    else:
                        if array[i][j-1]:
                            result[i][j] = random.choice(['01','02','03'])
                        else:
                            result[i][j] = '05'
                elif i == 0:
                    if array[i+1][j]:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                if not array[i+1][j+1]:
                                    result[i][j] = '13'
                                elif not array[i+1][j-1]:
                                    result[i][j] = '14'
                                else:
                                    result[i][j] = '00'
                            else:
                                result[i][j] = '09'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '10'
                            else:
                                # bad
                                result[i][j] = '00'
                    else:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                result[i][j] = '06'
                            else:
                                result[i][j] = '07'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '08'
                            else:
                                # bad
                                result[i][j] = '00'
                elif i == h-1:
                    if array[i-1][j]:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                if not array[i-1][j+1]:
                                    result[i][j] = '11'
                                elif not array[i-1][j-1]:
                                    result[i][j] = '12'
                                else:
                                    result[i][j] = '00'
                            else:
                                result[i][j] = '09'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '10'
                            else:
                                # bad
                                result[i][j] = '00'
                    else:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                result[i][j] = random.choice(['01','02','03'])
                            else:
                                result[i][j] = '04'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '05'
                            else:
                                # bad
                                result[i][j] = '00'
                elif j == 0:
                    if array[i][j+1]:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                if not array[i-1][j+1]:
                                    result[i][j] = '11'
                                elif not array[i+1][j+1]:
                                    result[i][j] = '13'
                                else:
                                    result[i][j] = '00'
                            else:
                                result[i][j] = random.choice(['01','02','03'])
                        else:
                            if array[i-1][j]:
                                result[i][j] = '06'
                            else:
                                # bad
                                result[i][j] = '00'
                    else:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                result[i][j] = '09'
                            else:
                                result[i][j] = '04'
                        else:
                            if array[i-1][j]:
                                result[i][j] = '07'
                            else:
                                # bad
                                result[i][j] = '00'
                elif j == w-1:
                    if array[i][j-1]:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                if not array[i-1][j-1]:
                                    result[i][j] = '12'
                                elif not array[i+1][j-1]:
                                    result[i][j] = '14'
                                else:
                                    result[i][j] = '00'
                            else:
                                result[i][j] = random.choice(['01','02','03'])
                        else:
                            if array[i-1][j]:
                                result[i][j] = '06'
                            else:
                                # bad
                                result[i][j] = '00'
                    else:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                result[i][j] = '10'
                            else:
                                result[i][j] = '05'
                        else:
                            if array[i-1][j]:
                                result[i][j] = '08'
                            else:
                                # bad
                                result[i][j] = '00'
                else:
                    if array[i][j-1]:
                        if array[i][j+1]:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    if not array[i-1][j+1]:
                                        result[i][j] = '11'
                                    elif not array[i+1][j+1]:
                                        result[i][j] = '13'
                                    elif not array[i-1][j-1]:
                                        result[i][j] = '12'
                                    elif not array[i+1][j-1]:
                                        result[i][j] = '14'
                                    else:
                                        result[i][j] = '00'
                                else:
                                    result[i][j] = random.choice(['01','02','03'])
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '06'
                                else:
                                    # bad
                                    result[i][j] = '00'
                        else:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    result[i][j] = '09'
                                else:
                                    result[i][j] = '04'
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '07'
                                else:
                                    # bad
                                    result[i][j] = '00'
                    else:
                        if array[i][j+1]:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    result[i][j] = '10'
                                else:
                                    result[i][j] = '05'
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '08'
                                else:
                                    # bad
                                    result[i][j] = '00'
                        else:
                            # bad
                            result[i][j] = '00'
            # if it's a white square
            else:
                if i == 0 and j == 0:
                    if array[i+1][j]:
                        if array[i][j+1]:
                            result[i][j] = '19'
                        else:
                            result[i][j] = random.choice(['16','17','18'])
                    else:
                        if array[i][j+1]:
                            result[i][j] = '24'
                        else:
                            if array[i+1][j+1]:
                                result[i][j] = '28'
                            else:
                                result[i][j] = '15'
                elif i == 0 and j == w-1:
                    if array[i+1][j]:
                        if array[i][j-1]:
                            result[i][j] = '20'
                        else:
                            result[i][j] = random.choice(['16','17','18'])
                    else:
                        if array[i][j-1]:
                            result[i][j] = '25'
                        else:
                            if array[i+1][j-1]:
                                result[i][j] = '29'
                            else:
                                result[i][j] = '15'
                elif i == h-1 and j == 0:
                    if array[i-1][j]:
                        if array[i][j+1]:
                            result[i][j] = '22'
                        else:
                            result[i][j] = '21'
                    else:
                        if array[i][j+1]:
                            result[i][j] = '24'
                        else:
                            if array[i-1][j+1]:
                                result[i][j] = '26'
                            else:
                                result[i][j] = '15'
                elif i == h-1 and j == w-1:
                    if array[i-1][j]:
                        if array[i][j-1]:
                            result[i][j] = '23'
                        else:
                            result[i][j] = '21'
                    else:
                        if array[i][j-1]:
                            result[i][j] = '25'
                        else:
                            if array[i+1][j+1]:
                                result[i][j] = '27'
                            else:
                                result[i][j] = '15'
                elif i == 0:
                    if array[i+1][j]:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '20'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '19'
                            else:
                                result[i][j] = random.choice(['16','17','18'])
                    else:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '25'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '24'
                            else:
                                if array[i+1][j+1]:
                                    result[i][j] = '28'
                                elif array[i+1][j-1]:
                                    result[i][j] = '29'
                                else:
                                    result[i][j] = '15'
                elif i == h-1:
                    if array[i-1][j]:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '23'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '22'
                            else:
                                result[i][j] = '21'
                    else:
                        if array[i][j-1]:
                            if array[i][j+1]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '25'
                        else:
                            if array[i][j+1]:
                                result[i][j] = '24'
                            else:
                                if array[i-1][j+1]:
                                    result[i][j] = '26'
                                elif array[i-1][j-1]:
                                    result[i][j] = '27'
                                else:
                                    result[i][j] = '15'
                elif j == 0:
                    if array[i][j+1]:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '19'
                        else:
                            if array[i-1][j]:
                                result[i][j] = '22'
                            else:
                                result[i][j] = '24'
                    else:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = random.choice(['16','17','18'])
                        else:
                            if array[i-1][j]:
                                result[i][j] = '21'
                            else:
                                if array[i-1][j+1]:
                                    result[i][j] = '26'
                                elif array[i+1][j+1]:
                                    result[i][j] = '28'
                                else:
                                    result[i][j] = '15'
                elif j == w-1:
                    if array[i][j-1]:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = '20'
                        else:
                            if array[i-1][j]:
                                result[i][j] = '23'
                            else:
                                result[i][j] = '25'
                    else:
                        if array[i+1][j]:
                            if array[i-1][j]:
                                # bad
                                result[i][j] = '15'
                            else:
                                result[i][j] = random.choice(['16','17','18'])
                        else:
                            if array[i-1][j]:
                                result[i][j] = '21'
                            else:
                                if array[i-1][j-1]:
                                    result[i][j] = '27'
                                elif array[i+1][j-1]:
                                    result[i][j] = '29'
                                else:
                                    result[i][j] = '15'
                else:
                    if array[i][j-1]:
                        if array[i][j+1]:
                            # bad
                            result[i][j] = '15'
                        else:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    # bad
                                    result[i][j] = '15'
                                else:
                                    result[i][j] = '20'
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '23'
                                else:
                                    result[i][j] = '25'
                    else:
                        if array[i][j+1]:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    # bad
                                    result[i][j] = '15'
                                else:
                                    result[i][j] = '19'
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '22'
                                else:
                                    result[i][j] = '24'
                        else:
                            if array[i+1][j]:
                                if array[i-1][j]:
                                    # bad
                                    result[i][j] = '15'
                                else:
                                    result[i][j] = random.choice(['16','17','18'])
                            else:
                                if array[i-1][j]:
                                    result[i][j] = '21'
                                else:
                                    if array[i-1][j-1]:
                                        result[i][j] = '27'
                                    elif array[i-1][j+1]:
                                        result[i][j] = '26'
                                    elif array[i+1][j-1]:
                                        result[i][j] = '29'
                                    elif array[i+1][j+1]:
                                        result[i][j] = '28'
                                    else:
                                        result[i][j] = '15'
    return result

def yes_no(screen, prompt):
    myfont = pygame.font.SysFont('Times New Roman', 30)
    s = " (y/n)"
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    sys.exit()
                elif event.key == pygame.K_y:
                    return True
                elif event.key == pygame.K_n:
                    return False
        textsurface = myfont.render(prompt+s, False, (0, 0, 0))
        tex_w, tex_h = myfont.size(prompt+s)
        scr_w, scr_h = screen.get_width(), screen.get_height()

        screen.fill(white)
        screen.blit(textsurface, (scr_w/2-tex_w/2,scr_h/2-tex_h/2))
        pygame.display.flip()

if __name__ == "__main__":

    # initialize fonts and screen (and clock)
    pygame.init()
    pygame.font.init()
    clock = pygame.time.Clock()
    myfont = pygame.font.SysFont('Times New Roman', 30)
    size = 1080, 720
    white = 255, 255, 255

    # ask if user wants to load
    screen = pygame.display.set_mode(size)
    if not yes_no(screen, "load file?"):
        # get level dimensions from user
        w = int(get_str(screen, "width: "))
        h = int(get_str(screen, "height: "))
        tileset = Tileset(w, h)
    else:
        # get level number to load
        lvl = str(int(get_str(screen, "level number: "))).zfill(2)
        with open("resources/level-files/level"+lvl+".lvl", "r") as fh:
            r, g, b = [int(x) for x in next(fh).split()]
            w, h = [int(x) for x in next(fh).split()]
            array = [0] * h
            for i in range(h):
                array[i] = [int(x) < 15 for x in next(fh).split()]
            num_chars = int(next(fh))
            obj_array = []
            for i in range(num_chars):
                char_x, char_y = [int(x) for x in next(fh).split()]
                level_end_x, level_end_y = [int(x) for x in next(fh).split()]
                obj_array.append(["char", char_x, char_y, not i])
                obj_array.append(["level_end", level_end_x, level_end_y, not i])

        tileset = Tileset(w, h, array, obj_array)

    # create the border, gridlines, tileset, and camera
    border = Border(w, h)
    grid = Gridlines(w, h)
    camera = Camera(w*TILE_WIDTH, h*TILE_WIDTH, screen.get_width(), screen.get_height())
    done = False
    placing = 0

    while not done:
        clock.tick(100)
        camrect = camera.get_rect()

        # handle events (check for quit signal)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                elif event.key == pygame.K_RETURN:
                    done = True
                elif event.key == pygame.K_s:
                    tileset.add_row_bottom()
                    h += 1
                elif event.key == pygame.K_w:
                    camera.move_down()
                    tileset.add_row_top()
                    h += 1
                elif event.key == pygame.K_d:
                    tileset.add_col_right()
                    w += 1
                elif event.key == pygame.K_a:
                    camera.move_right()
                    tileset.add_col_left()
                    w += 1
                elif event.key == pygame.K_i:
                    if h > 2:
                        tileset.remove_row_bottom()
                        h -= 1
                elif event.key == pygame.K_k:
                    if h > 2:
                        camera.move_up()
                        tileset.remove_row_top()
                        h -= 1
                elif event.key == pygame.K_j:
                    if w > 2:
                        tileset.remove_col_right()
                        w -= 1
                elif event.key == pygame.K_l:
                    if w > 2:
                        camera.move_left()
                        tileset.remove_col_left()
                        w -= 1
                # placing tiles
                elif event.key == pygame.K_0:
                    placing = 0
                # placing chars
                elif event.key == pygame.K_1:
                    placing = 1
                # placing level ends
                elif event.key == pygame.K_2:
                    placing = 2
                # placing buttons
                elif event.key == pygame.K_3:
                    placing = 3
                # placing springs
                elif event.key == pygame.K_4:
                    placing = 4
                elif event.key == pygame.K_BACKSPACE:
                    placing = -1

            camera.handle_event(event)
            tileset.handle_event(event, screen, camrect, placing)

        # update the objects that change
        camera.update(screen, w, h)
        border.update(w, h)
        grid.update(w, h)

        camrect = camera.get_rect()

        # draw everything to the screen
        screen.fill(white)
        tileset.draw(screen, camrect)
        border.draw(screen, camrect)
        grid.draw(screen, camrect)
        pygame.display.flip()

    # get filename and output array
    filename = "level"+get_str(screen, "level number: ").zfill(2)+".lvl"
    palette_red = int(get_str(screen, "red: "))
    palette_green = int(get_str(screen, "green: "))
    palette_blue = int(get_str(screen, "blue: "))
    tiles = output_arr(tileset.get_array())
    objects = tileset.get_obj_array()

    w, h = len(tiles[0]), len(tiles)

    # output .lvl file!
    with open('resources/level-files/'+filename, 'w') as fh:
        fh.write(str(palette_red)+' '+str(palette_green)+' '+str(palette_blue)+'\n')
        fh.write(str(w)+' '+str(h)+'\n')
        for i in range(h):
            for j in range(w):
                fh.write(tiles[i][j])
                if j != w-1:
                    fh.write(' ')
            fh.write('\n')
        num_chars = 0
        chars = [0,0]
        level_ends = [0,0]
        for i in range(len(objects)):
            if objects[i][0] == "char":
                num_chars += 1
                chars[not objects[i][3]] = (objects[i][1], objects[i][2])
            if objects[i][0] == "level_end":
                level_ends[not objects[i][3]] = (objects[i][1], objects[i][2])
        fh.write(str(num_chars)+'\n')
        for i in range(num_chars):
            fh.write(str(chars[i][0])+' '+str(chars[i][1])+'\n')
            fh.write(str(level_ends[i][0])+' '+str(level_ends[i][1])+'\n')
