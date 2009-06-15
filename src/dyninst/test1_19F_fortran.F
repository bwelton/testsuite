 !
 ! Copyright (c) 1996-2004 Barton P. Miller
 ! 
 ! We provide the Paradyn Parallel Performance Tools (below
 ! described as "Paradyn") on an AS IS basis, and do not warrant its
 ! validity or performance.  We reserve the right to update, modify,
 ! or discontinue this software at any time.  We shall have no
 ! obligation to supply such updates or modifications or any other
 ! form of support to you.
 ! 
 ! This license is for research uses.  For such uses, there is no
 ! charge. We define "research use" to mean you may freely use it
 ! inside your organization for whatever purposes you see fit. But you
 ! may not re-distribute Paradyn or parts of Paradyn, in any form
 ! source or binary (including derivatives), electronic or otherwise,
 ! to any other organization or entity without our permission.
 ! 
 ! (for other uses, please contact us at paradyn@cs.wisc.edu)
 ! 
 ! All warranties, including without limitation, any warranty of
 ! merchantability or fitness for a particular purpose, are hereby
 ! excluded.
 ! 
 ! By your use of Paradyn, you understand and agree that we (or any
 ! other person or entity with proprietary rights in Paradyn) are
 ! under no obligation to provide either maintenance services,
 ! update services, notices of latent defects, or correction of
 ! defects for Paradyn.
 ! 
 ! Even if advised of the possibility of such damages, under no
 ! circumstances shall we (or any other person or entity with
 ! proprietary rights in the software licensed hereunder) be liable
 ! to you or any third party for direct, indirect, or consequential
 ! damages of any character regardless of type of action, including,
 ! without limitation, loss of profits, loss of use, loss of good
 ! will, or computer failure or malfunction.  You agree to indemnify
 ! us (and any other person or entity with proprietary rights in the
 ! software licensed hereunder) for any and all liability it may
 ! incur to third parties resulting from your use of Paradyn.
 !

! libdyninst validation suite test #1
! author: Jon Beyer (6/1/01)
!	derived from a previous test by Jeff Hollingsworth

!------------------------------------------------------------------------------

! AIX xlf90 does not insert underscores at end

#if !defined(XLF)
#define stop_process_ stop_process
#endif

#ifdef __GNUC__
#define AND iand
#define OR ior
#else
#define AND and
#define OR or
#endif

!------------------------------------------------------------------------------

	SUBROUTINE test1_19f_init_globals
#include "test1_19F_common.h"

	globalVariable19_1 = Z'deadbeef'
	globalVariable19_2 = z'deadbeef'

	END

!------------------------------------------------------------------------------

	SUBROUTINE func19_1
#include "test1_19F_common.h"

	CALL stop_process_

	if (globalVariable19_1 .ne. 1900100) then
		print *, '**Failed test #19 (oneTimeCode)'
		print *, '     globalVariable19_1 contained ',
     & globalVariable19_1, ', not 1900100 as expected'
	endif

	CALL stop_process_

	if (globalVariable19_2 .eq. 1900200) then
		passedTest = 1
	else
		print *, '**Failed test #19 (oneTimeCode)'
		print *, '     globalVariable19_2 contained ',
     & globalVariable19_2, ', not 1900200 as expected'
	endif
	END

!------------------------------------------------------------------------------

	SUBROUTINE test1_19_call1
#include "test1_19F_common.h"

	globalVariable19_1 = 1900100
	END

!------------------------------------------------------------------------------

	SUBROUTINE test1_19_call2
#include "test1_19F_common.h"

	globalVariable19_2 = 1900200
	END

!------------------------------------------------------------------------------