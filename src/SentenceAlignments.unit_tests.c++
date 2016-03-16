#include "catch.h++"
#include "SentenceAlignments.h++"

/*
 
 a1 
 b1 c1
 d1 e1
 f1 g1 h1 i1 j1 k1 l1

 alignment of 1 with 2 => {1, 2, -1, 3, -1, 4, -1}
 
 a2 b2 c2
 d2 e2
 f2 g2 h2 i2 j2 k2 l2
 
 alignment of 2 with 1 => {1, -1, 2, -1, 3, -1}
 

 a3 b3 c3 d3 e3
 f3 g3 h3 i3 j3 k3 l3

 alignment of 3       with 1 and 2 => {1, -1, 2, -1}
 alignment of 1 and 2 with 3       => {1, 2, -1, 3, -1}
 
 alignment of 1 with 2 and 3 after adding 3 => {1, 2, 3, -1, 4, -1}
 alignment of 2 with 1 and 3 after adding 3 => {1, 2, -1, 4, -1}
 alignment of 3 with 1 and 2 after adding 3 => {1, -1, 2, -1}
 
 a4 b4 c4 d4 e4 f4 g4 h4 i4 j4 k4 l4
 
 {1, -1}
 
 */

SCENARIO( "Sentence alignments can be merged", "[SentenceAlignments]" ) {
  
  GIVEN( "Alignments of language 1 and language 2" ) {
  
    std::vector<int> alignment_of_1_with_2 {1, 2, -1, 3, -1, 4, -1};
    std::vector<int> alignment_of_2_with_1 {1, -1, 2, -1, 3, -1};
    
    WHEN( "further aligned with language 3" ) {
      std::vector<int> alignment_of_3_with_1_and_2 {1, -1, 2, -1};
      std::vector<int> alignment_of_1_and_2_with_3 {1, 2, -1, 3, -1};
      
      std::vector<int> alignment_of_1_with_2_and_3 {1, 2, 3, -1, 4, -1};
      std::vector<int> alignment_of_2_with_1_and_3 {1, 2, -1, 3, -1};

      THEN("results are as expected") {
        REQUIRE(alignment_of_1_with_2_and_3 == SentenceAlignments::merge(alignment_of_1_with_2, alignment_of_1_and_2_with_3));
        REQUIRE(alignment_of_2_with_1_and_3 == SentenceAlignments::merge(alignment_of_2_with_1, alignment_of_1_and_2_with_3));
      }
      
    }
  }
}



/*
SCENARIO( "Sentence alignments can be merged", "[SentenceAlignments]" ) {
  
  GIVEN( "A simple existing vector representing one side of an alignment" ) {
    std::vector<int> previouslyAlignedValues{ 1, -1, 2, -1, 3, -1, 4, -1 };
    
    WHEN( "expanded with an identical vector" ) {
      std::vector<int> newlyAlignedValues{ 1, -1, 2, -1, 3, -1, 4, -1};
      std::vector<int> results = SentenceAlignments::merge(previouslyAlignedValues, newlyAlignedValues);
      
      THEN( "nothing changes" ) {
        REQUIRE( previouslyAlignedValues.size() == results.size() );
        REQUIRE( previouslyAlignedValues == results );
      }
    }

    WHEN( "results match newly aligned values" ) {
      std::vector<int> newlyAlignedValues{ 1, -1, 2, 3, 4, -1 };
      std::vector<int> results = SentenceAlignments::merge(previouslyAlignedValues, newlyAlignedValues);
      
      THEN( "nothing changes" ) {
        REQUIRE( newlyAlignedValues.size() == results.size() );
        REQUIRE( newlyAlignedValues == results );
      }
    }
    
    WHEN( "all values are merged" ) {
      std::vector<int> newlyAlignedValues{ 1, 2, 3, 4, -1 };
      std::vector<int> results = SentenceAlignments::merge(previouslyAlignedValues, newlyAlignedValues);
      
      THEN( "results match newly aligned values" ) {
        REQUIRE( newlyAlignedValues.size() == results.size() );
        REQUIRE( newlyAlignedValues == results );
      }
    }
    
  }
  
  GIVEN( "A slightly more complicated existing vector representing one side of an alignment" ) {
    std::vector<int> previouslyAlignedValues{ 1, 2, -1, 3, -1, 4, -1 };
    
    WHEN( "expanded with a simple vector" ) {
      std::vector<int> newlyAlignedValues{ 1, -1, 2, -1, 3, -1 };
      std::vector<int> results = SentenceAlignments::merge(previouslyAlignedValues, newlyAlignedValues);
      
      THEN( "nothing changes" ) {
        REQUIRE( previouslyAlignedValues.size() == results.size() );
        REQUIRE( previouslyAlignedValues == results );
      }
    }
    
    WHEN( "expanded with a fully collapsing vector" ) {
      std::vector<int> newlyAlignedValues{ 1, -1 };
      std::vector<int> results = SentenceAlignments::merge(previouslyAlignedValues, newlyAlignedValues);
      
      std::vector<int> expectedResults{ 1, 2, 3, 4, -1 };
      
      THEN( "we obtain the expected results" ) {
        REQUIRE( expectedResults.size() == results.size() );
        REQUIRE( expectedResults == results );
      }
    }
    
  }
}
*/

