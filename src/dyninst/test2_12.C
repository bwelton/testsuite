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

// $Id: test2_12.C,v 1.1 2008/10/30 19:20:14 legendre Exp $
/*
 * #Name: test2_12
 * #Desc: BPatch_point query funcs
 * #Dep: 
 * #Arch:
 * #Notes:
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"

#include "dyninst_comp.h"
class test2_12_Mutator : public DyninstMutator {
  virtual test_results_t executeTest();
};
extern "C" DLLEXPORT  TestMutator *test2_12_factory() {
  return new test2_12_Mutator();
}

//
// Test #12 - BPatch_point query funcs
//	This tests the BPatch_point functions that supply information about
//	an inst. point:
//		getAddress - should return the address of the point
//		usesTrap_NP - returns true of the point requires a trap
//			instruction.

// This test looks like it also does nothing..  - Greg

// static int mutatorTest(BPatch_thread *appThread, BPatch_image *appImage)
test_results_t test2_12_Mutator::executeTest() {
  BPatch_Vector<BPatch_function *> found_funcs;
  char *funcname = "test2_12_func1";
    if ((NULL == appImage->findFunction(funcname, found_funcs, 1)) || !found_funcs.size()) {
      logerror("    Unable to find function %s\n", funcname);
      return FAILED;
    }

    if (1 < found_funcs.size()) {
      logerror("%s[%d]:  WARNING  : found %d functions named %s.  Using the first.\n", 
	      __FILE__, __LINE__, found_funcs.size(), funcname);
    }

    BPatch_Vector<BPatch_point *> *points = found_funcs[0]->findPoint(BPatch_entry);
    
    if (!points) {
	logerror("**Failed** test #12 (BPatch_point query funcs)\n");
	logerror("    unable to locate function \"%s\".\n", funcname);
        return FAILED;
    }

    //void *addr = (*points)[0]->getAddress();

    //bool trapFlag = 
    (*points)[0]->usesTrap_NP();

    // Set the variable test2_12_passed in the mutatee
    BPatch_variableExpr *passed_expr =
      appImage->findVariable("test2_12_passed");
    if (passed_expr == NULL) {
      logerror("**Failed** test #12 (BPatch_point query funcs)\n");
      logerror("    Unable to locate test2_12_passed\n");
      return FAILED;
    } else {
      int pvalue = 1;
      passed_expr->writeValue(&pvalue);
      logerror("Passed test #12 (BPatch_point query funcs)\n");
      return PASSED;
    }
}

// extern "C" TEST_DLL_EXPORT int test2_12_mutatorMAIN(ParameterDict &param)
// {
//     bool useAttach = param["useAttach"]->getInt();
//     BPatch *bpatch = (BPatch *)(param["bpatch"]->getPtr());

//     BPatch_thread *appThread = (BPatch_thread *)(param["appThread"]->getPtr());

//     // Get log file pointers
//     FILE *outlog = (FILE *)(param["outlog"]->getPtr());
//     FILE *errlog = (FILE *)(param["errlog"]->getPtr());
//     setOutputLog(outlog);
//     setErrorLog(errlog);

//     // Read the program's image and get an associated image object
//     BPatch_image *appImage = appThread->getImage();

//     // Signal the child that we've attached
//     if (useAttach) {
// 	signalAttached(appThread, appImage);
//     }

//     // This calls the actual test to instrument the mutatee
//     return mutatorTest(appThread, appImage);
// }