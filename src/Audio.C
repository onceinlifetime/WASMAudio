// Copyright (c) 2017-2018 The WASM audio Authors. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of mad chops coder AU nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Audio.H"
#include <stdio.h>

#include <Eigen/Dense>

Audio::Audio(){
  printf("Audio constructed\n");
}

Audio::~Audio(){
  printf("Audio destructed\n");
}

bool Audio::process(intptr_t input, unsigned int Min, unsigned int Nin, intptr_t output, unsigned int Mout, unsigned int Nout){
  float *in=(float*)input, *out = (float*)output;
  if (Nin!=Nout){
    printf("Input and output sample count mismatch : Nin = %d, Nout = %d\n", Nin, Nout);
    return false;
  }

  Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Unaligned >
                                          inAudio((float*)input, Nin, Min),
                                          outAudio((float*)output, Nout, Mout);

  unsigned int M = ((Min<Mout) ? Min : Mout);
  outAudio.leftCols(M)=inAudio.leftCols(M); // simply copy the audio over
  if (outAudio.cols()>M)
    outAudio.rightCols(outAudio.cols()-M).setZero();
  return true;
}

#include "config.h"
#ifdef HAVE_EMSCRIPTEN
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(Audio_ex) {
  emscripten::class_<Audio>("Audio")
  .constructor()
  .function("process", &Audio::process, emscripten::allow_raw_pointers())
  ;
}
#endif
