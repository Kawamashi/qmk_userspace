/* Copyright 2025 @Kawamashi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define KAWA_LAYOUT(\
             k20, k21, k22, k23, k24,           k25, k26, k27, k28, k29,\
             k30, k31, k32, k33, k34,           k35, k36, k37, k38, k39,\
             k40, k41, k42, k43, k44,           k45, k46, k47, k48, k49,\
                            k51, k52, k53, k54, k55, k56)\
    LAYOUT_split_3x5_3(\
             k20, k21, k22, k23, k24,           k25, k26, k27, k28, k29,\
             k30, k31, k32, k33, k34,           k35, k36, k37, k38, k39,\
             k40, k41, k42, k43, k44,           k45, k46, k47, k48, k49,\
                            k51, k52, k53, k54, k55, k56)
                            

#define CHORDAL_HOLD_KAWA_LAYOUT(\
             k20, k21, k22, k23, k24,           k25, k26, k27, k28, k29,\
             k30, k31, k32, k33, k34,           k35, k36, k37, k38, k39,\
             k40, k41, k42, k43, k44,           k45, k46, k47, k48, k49,\
                            k51, k52, k53, k54, k55, k56)\
    LAYOUT_split_3x5_3(\
             k20, k21, k22, k23, k24,           k25, k26, k27, k28, k29,\
             k30, k31, k32, k33, k34,           k35, k36, k37, k38, k39,\
             k40, k41, k42, k43, k44,           k45, k46, k47, k48, k49,\
                            k51, k52, k53, k54, k55, k56)

#define LEFT_HOME_ROW 2