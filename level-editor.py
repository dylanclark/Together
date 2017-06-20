#!/usr/bin/env python

# controls:
#   to fill a rectangle of squares -
#       (r/l)click the two corners of the rectangle
#   add a row - h (TODO)
#   delete a row - j (TODO)
#   add a column - k (TODO)
#   delete a column - l (TODO)
#   output space separated file - enter

import pygame
import sys
import random

TILE_WIDTH = 16

class Camera:
    truex, truey, truew, trueh = 0, 0, 0, 0
    locx, locy, locw, loch = 0, 0, 0, 0
    up, down, left, right = False, False, False, False
    zoomout, zoomin = False, False

    def __init__(self, w, h):
        self.locx, self.locy = float(w)/2., float(h)/2.
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

class Tileset:
    array = []
    rect = 0
    x1, y1 = 0, 0

    def __init__(self, w, h, array=None):
        if not array:
            self.array = [[False]*w for _ in range(h)]
        else:
            self.array = array

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


    def handle_event(self, event, screen, cam):
        scr_w, scr_h = screen.get_width(), screen.get_height()
        camx, camy, camw, camh = cam
        if event.type == pygame.MOUSEBUTTONDOWN:
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

    def fill_rect(self, black, mx, my):
        x1, x2 = min(self.x1, mx), max(self.x1, mx)
        y1, y2 = min(self.y1, my), max(self.y1, my)
        for i in range(y1, y2+1):
            for j in range(x1, x2+1):
                self.array[i][j] = black
        self.rect = 0

    def add_rows(self, n):
        w = len(self.array[0])
        self.array = self.array + [[False]*w for _ in range(n)]

    def remove_rows(self, n):
        self.array = self.array[:-n]

    def add_cols(self, n):
        pass

    def remove_cols(self, n):
        pass

    def get_array(self):
        return self.array

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

    # get level dimensions from user
    screen = pygame.display.set_mode(size)
    if not yes_no(screen, "load file?"):
        w = int(get_str(screen, "width: "))
        h = int(get_str(screen, "height: "))
        tileset = Tileset(w, h)
    else:
        lvl = str(int(get_str(screen, "level number: "))).zfill(2)
        with open("resources/level-files/level"+lvl+".lvl", "r") as fh:
            w, h = [int(x) for x in next(fh).split()]
            array = [[int(x) < 15 for x in line.split()] for line in fh]
        tileset = Tileset(w, h, array)

    # create the border, gridlines, tileset, and camera
    border = Border(w, h)
    grid = Gridlines(w, h)
    camera = Camera(screen.get_width(), screen.get_height())
    done = False

    while not done:
        camrect = camera.get_rect()
        clock.tick(100)

        # handle events (check for quit signal)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    done = True
            camera.handle_event(event)
            tileset.handle_event(event, screen, camrect)

        # update the objects that change
        camera.update(screen, w, h)

        # draw everything to the screen
        screen.fill(white)
        tileset.draw(screen, camrect)
        border.draw(screen, camrect)
        grid.draw(screen, camrect)
        pygame.display.flip()

    # get filename and output array
    filename = "level"+get_str(screen, "level number: ").zfill(2)+".lvl"
    output = output_arr(tileset.get_array())
    w, h = len(output[0]), len(output)

    # output .lvl file!
    with open('resources/level-files/'+filename, 'w') as fh:
        fh.write(str(w)+' '+str(h)+'\n')
        for i in range(h):
            for j in range(w):
                fh.write(output[i][j])
                if j != w-1:
                    fh.write(' ')
            if i != h-1:
                fh.write('\n')
