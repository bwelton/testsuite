/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

// $Id: test2_3.C,v 1.1 2008/10/30 19:20:21 legendre Exp $
/*
 * #Name: test2_3
 * #Desc: Attatch to an invalid pid
 * #Dep: 
 * #Arch: !sparc_sun_sunos4_1_3_test
 * #Notes:
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"
#include "Callbacks.h"

#include "dyninst_comp.h"
class test2_3_Mutator : public DyninstMutator {
  BPatch *bpatch;

  virtual bool hasCustomExecutionPath() { return true; }
  virtual test_results_t setup(ParameterDict &param);
  virtual test_results_t executeTest();
};
extern "C" DLLEXPORT  TestMutator *test2_3_factory() {
  return new test2_3_Mutator();
}

//
// Test #3 - attach to an invalid pid
//	Try to attach to an invalid pid number (65539).
//
// static int mutatorTest(BPatch *bpatch, bool useAttach) 
test_results_t test2_3_Mutator::executeTest() {
    // attach to an an invalid pid
    clearError();
    BPatch_thread *ret = bpatch->attachProcess(NULL, 65539);
    int gotError = getError();
    if (ret || !gotError) {
	logerror("**Failed** test #3 (attach to an invalid pid)\n");
	if (ret)
    	    logerror("    created a thread handle for invalid executable\n");
	if (!gotError)
	    logerror("    the error callback should have been called but wasn't\n");
        return FAILED;
    } else {
	logerror("Passed test #3 (attach to an invalid pid)\n");
        return PASSED;
    }
}

// extern "C" TEST_DLL_EXPORT int test2_3_mutatorMAIN(ParameterDict &param)
test_results_t test2_3_Mutator::setup(ParameterDict &param) {
  int useAttach = param["useAttach"]->getInt();
  bpatch = (BPatch *)(param["bpatch"]->getPtr());
  
  // Get log file pointers
//   FILE * outlog = (FILE *)(param["outlog"]->getPtr());
//   FILE *errlog = (FILE *)(param["errlog"]->getPtr());
//   setOutputLog(outlog);
//   setErrorLog(errlog);

#if defined(sparc_sun_sunos4_1_3_test)
    logerror("Skipping test #3 (attach to an invalid pid)\n");
    logerror("    attach is not supported on this platform\n");
    return SKIPPED;
#else
    if ( !useAttach ) {
      logerror("Skipping test #3 (attach to an invalid pid)\n");
      logerror("    test only makes sense in attach mode\n");
      return SKIPPED;
    }
#endif
  return PASSED;

}