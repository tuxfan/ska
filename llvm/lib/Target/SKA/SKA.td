//===-- SKA.td - Describe the SKA Target Machine -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "SKARegisterInfo.td"
include "SKACallingConv.td"
include "SKAInstrInfo.td"

def SKAInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// SPARC processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic",         []>;
def : Proc<"v8",              []>;
def : Proc<"supersparc",      []>;
def : Proc<"sparclite",       []>;
def : Proc<"f934",            []>;
def : Proc<"hypersparc",      []>;
def : Proc<"sparclite86x",    []>;
def : Proc<"sparclet",        []>;
def : Proc<"tsc701",          []>;
def : Proc<"v9",              [FeatureV9]>;
def : Proc<"ultrasparc",      [FeatureV9, FeatureV8Deprecated]>;
def : Proc<"ultrasparc3",     [FeatureV9, FeatureV8Deprecated]>;
def : Proc<"ultrasparc3-vis", [FeatureV9, FeatureV8Deprecated, FeatureVIS]>;


//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def SKA : Target {
  // Pull in Instruction Info:
  let InstructionSet = SKAInstrInfo;
}
