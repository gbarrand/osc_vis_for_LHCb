
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>

#include <Lib/Arguments.h>
#include <Lib/Printer.h>
#include <Lib/Debug.h>
#include <Lib/smanip.h>
#include <inlib/cast>
#include <Lib/Time.h>

#include <BatchLab/Interfaces/ITupleColumn.h>
#include <BatchLab/Core/MemoryTuple.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Arguments args(aArgc,aArgv);

  int verboseLevel = 0;
  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  std::string test;
  args.find("-test",test);

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return 0;

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) return 0;
  AIDA::ITree* tree = treeFactory->create();
  delete treeFactory;
  if(!tree) return 0;

  // Create a tuple :
  int nints = 100;
  int nfloats = 100;
  int rown = 200000;

  int count;
  int icol;
  std::string scols;
  for(icol=0;icol<nints;icol++) {
    if(icol) scols+= ",";
    std::string s;
    Lib::smanip::printf(s,32,"%d",icol);
    scols += "int v"+s;
  }
  for(icol=0;icol<nfloats;icol++) {
    scols+= ",";
    std::string s;
    Lib::smanip::printf(s,32,"%d",nints+icol);
    scols += "float v"+s;
  }
  //std::cout << scols << std::endl;

  AIDA::ITupleFactory* tupleFactory = aida->createTupleFactory(*tree);
  if(!tupleFactory) return 0;
  AIDA::ITuple* tuple = tupleFactory->create("tuple","tuple",scols);
  delete tupleFactory;
  if(!tuple) return 0;

  printf("debug : test : \"%s\"\n",test.c_str());

  if(test=="tc") {

    BatchLab::MemoryTuple* memTuple =  INLIB_CAST(*tuple,BatchLab::MemoryTuple);

    BatchLab::ITupleColumn* col_0 = memTuple->findColumn1(0)->tupleColumn();
    BatchLab::ITupleColumn* col_1 = memTuple->findColumn1(1)->tupleColumn();
    BatchLab::ITupleColumn* col_2 = memTuple->findColumn1(2)->tupleColumn();
    BatchLab::ITupleColumn* col_3 = memTuple->findColumn1(3)->tupleColumn();
    BatchLab::ITupleColumn* col_4 = memTuple->findColumn1(4)->tupleColumn();
    BatchLab::ITupleColumn* col_5 = memTuple->findColumn1(5)->tupleColumn();
    BatchLab::ITupleColumn* col_6 = memTuple->findColumn1(6)->tupleColumn();
    BatchLab::ITupleColumn* col_7 = memTuple->findColumn1(7)->tupleColumn();
    BatchLab::ITupleColumn* col_8 = memTuple->findColumn1(8)->tupleColumn();
    BatchLab::ITupleColumn* col_9 = memTuple->findColumn1(9)->tupleColumn();
    BatchLab::ITupleColumn* col_10 = memTuple->findColumn1(10)->tupleColumn();
    BatchLab::ITupleColumn* col_11 = memTuple->findColumn1(11)->tupleColumn();
    BatchLab::ITupleColumn* col_12 = memTuple->findColumn1(12)->tupleColumn();
    BatchLab::ITupleColumn* col_13 = memTuple->findColumn1(13)->tupleColumn();
    BatchLab::ITupleColumn* col_14 = memTuple->findColumn1(14)->tupleColumn();
    BatchLab::ITupleColumn* col_15 = memTuple->findColumn1(15)->tupleColumn();
    BatchLab::ITupleColumn* col_16 = memTuple->findColumn1(16)->tupleColumn();
    BatchLab::ITupleColumn* col_17 = memTuple->findColumn1(17)->tupleColumn();
    BatchLab::ITupleColumn* col_18 = memTuple->findColumn1(18)->tupleColumn();
    BatchLab::ITupleColumn* col_19 = memTuple->findColumn1(19)->tupleColumn();
    BatchLab::ITupleColumn* col_20 = memTuple->findColumn1(20)->tupleColumn();
    BatchLab::ITupleColumn* col_21 = memTuple->findColumn1(21)->tupleColumn();
    BatchLab::ITupleColumn* col_22 = memTuple->findColumn1(22)->tupleColumn();
    BatchLab::ITupleColumn* col_23 = memTuple->findColumn1(23)->tupleColumn();
    BatchLab::ITupleColumn* col_24 = memTuple->findColumn1(24)->tupleColumn();
    BatchLab::ITupleColumn* col_25 = memTuple->findColumn1(25)->tupleColumn();
    BatchLab::ITupleColumn* col_26 = memTuple->findColumn1(26)->tupleColumn();
    BatchLab::ITupleColumn* col_27 = memTuple->findColumn1(27)->tupleColumn();
    BatchLab::ITupleColumn* col_28 = memTuple->findColumn1(28)->tupleColumn();
    BatchLab::ITupleColumn* col_29 = memTuple->findColumn1(29)->tupleColumn();
    BatchLab::ITupleColumn* col_30 = memTuple->findColumn1(30)->tupleColumn();
    BatchLab::ITupleColumn* col_31 = memTuple->findColumn1(31)->tupleColumn();
    BatchLab::ITupleColumn* col_32 = memTuple->findColumn1(32)->tupleColumn();
    BatchLab::ITupleColumn* col_33 = memTuple->findColumn1(33)->tupleColumn();
    BatchLab::ITupleColumn* col_34 = memTuple->findColumn1(34)->tupleColumn();
    BatchLab::ITupleColumn* col_35 = memTuple->findColumn1(35)->tupleColumn();
    BatchLab::ITupleColumn* col_36 = memTuple->findColumn1(36)->tupleColumn();
    BatchLab::ITupleColumn* col_37 = memTuple->findColumn1(37)->tupleColumn();
    BatchLab::ITupleColumn* col_38 = memTuple->findColumn1(38)->tupleColumn();
    BatchLab::ITupleColumn* col_39 = memTuple->findColumn1(39)->tupleColumn();
    BatchLab::ITupleColumn* col_40 = memTuple->findColumn1(40)->tupleColumn();
    BatchLab::ITupleColumn* col_41 = memTuple->findColumn1(41)->tupleColumn();
    BatchLab::ITupleColumn* col_42 = memTuple->findColumn1(42)->tupleColumn();
    BatchLab::ITupleColumn* col_43 = memTuple->findColumn1(43)->tupleColumn();
    BatchLab::ITupleColumn* col_44 = memTuple->findColumn1(44)->tupleColumn();
    BatchLab::ITupleColumn* col_45 = memTuple->findColumn1(45)->tupleColumn();
    BatchLab::ITupleColumn* col_46 = memTuple->findColumn1(46)->tupleColumn();
    BatchLab::ITupleColumn* col_47 = memTuple->findColumn1(47)->tupleColumn();
    BatchLab::ITupleColumn* col_48 = memTuple->findColumn1(48)->tupleColumn();
    BatchLab::ITupleColumn* col_49 = memTuple->findColumn1(49)->tupleColumn();
    BatchLab::ITupleColumn* col_50 = memTuple->findColumn1(50)->tupleColumn();
    BatchLab::ITupleColumn* col_51 = memTuple->findColumn1(51)->tupleColumn();
    BatchLab::ITupleColumn* col_52 = memTuple->findColumn1(52)->tupleColumn();
    BatchLab::ITupleColumn* col_53 = memTuple->findColumn1(53)->tupleColumn();
    BatchLab::ITupleColumn* col_54 = memTuple->findColumn1(54)->tupleColumn();
    BatchLab::ITupleColumn* col_55 = memTuple->findColumn1(55)->tupleColumn();
    BatchLab::ITupleColumn* col_56 = memTuple->findColumn1(56)->tupleColumn();
    BatchLab::ITupleColumn* col_57 = memTuple->findColumn1(57)->tupleColumn();
    BatchLab::ITupleColumn* col_58 = memTuple->findColumn1(58)->tupleColumn();
    BatchLab::ITupleColumn* col_59 = memTuple->findColumn1(59)->tupleColumn();
    BatchLab::ITupleColumn* col_60 = memTuple->findColumn1(60)->tupleColumn();
    BatchLab::ITupleColumn* col_61 = memTuple->findColumn1(61)->tupleColumn();
    BatchLab::ITupleColumn* col_62 = memTuple->findColumn1(62)->tupleColumn();
    BatchLab::ITupleColumn* col_63 = memTuple->findColumn1(63)->tupleColumn();
    BatchLab::ITupleColumn* col_64 = memTuple->findColumn1(64)->tupleColumn();
    BatchLab::ITupleColumn* col_65 = memTuple->findColumn1(65)->tupleColumn();
    BatchLab::ITupleColumn* col_66 = memTuple->findColumn1(66)->tupleColumn();
    BatchLab::ITupleColumn* col_67 = memTuple->findColumn1(67)->tupleColumn();
    BatchLab::ITupleColumn* col_68 = memTuple->findColumn1(68)->tupleColumn();
    BatchLab::ITupleColumn* col_69 = memTuple->findColumn1(69)->tupleColumn();
    BatchLab::ITupleColumn* col_70 = memTuple->findColumn1(70)->tupleColumn();
    BatchLab::ITupleColumn* col_71 = memTuple->findColumn1(71)->tupleColumn();
    BatchLab::ITupleColumn* col_72 = memTuple->findColumn1(72)->tupleColumn();
    BatchLab::ITupleColumn* col_73 = memTuple->findColumn1(73)->tupleColumn();
    BatchLab::ITupleColumn* col_74 = memTuple->findColumn1(74)->tupleColumn();
    BatchLab::ITupleColumn* col_75 = memTuple->findColumn1(75)->tupleColumn();
    BatchLab::ITupleColumn* col_76 = memTuple->findColumn1(76)->tupleColumn();
    BatchLab::ITupleColumn* col_77 = memTuple->findColumn1(77)->tupleColumn();
    BatchLab::ITupleColumn* col_78 = memTuple->findColumn1(78)->tupleColumn();
    BatchLab::ITupleColumn* col_79 = memTuple->findColumn1(79)->tupleColumn();
    BatchLab::ITupleColumn* col_80 = memTuple->findColumn1(80)->tupleColumn();
    BatchLab::ITupleColumn* col_81 = memTuple->findColumn1(81)->tupleColumn();
    BatchLab::ITupleColumn* col_82 = memTuple->findColumn1(82)->tupleColumn();
    BatchLab::ITupleColumn* col_83 = memTuple->findColumn1(83)->tupleColumn();
    BatchLab::ITupleColumn* col_84 = memTuple->findColumn1(84)->tupleColumn();
    BatchLab::ITupleColumn* col_85 = memTuple->findColumn1(85)->tupleColumn();
    BatchLab::ITupleColumn* col_86 = memTuple->findColumn1(86)->tupleColumn();
    BatchLab::ITupleColumn* col_87 = memTuple->findColumn1(87)->tupleColumn();
    BatchLab::ITupleColumn* col_88 = memTuple->findColumn1(88)->tupleColumn();
    BatchLab::ITupleColumn* col_89 = memTuple->findColumn1(89)->tupleColumn();
    BatchLab::ITupleColumn* col_90 = memTuple->findColumn1(90)->tupleColumn();
    BatchLab::ITupleColumn* col_91 = memTuple->findColumn1(91)->tupleColumn();
    BatchLab::ITupleColumn* col_92 = memTuple->findColumn1(92)->tupleColumn();
    BatchLab::ITupleColumn* col_93 = memTuple->findColumn1(93)->tupleColumn();
    BatchLab::ITupleColumn* col_94 = memTuple->findColumn1(94)->tupleColumn();
    BatchLab::ITupleColumn* col_95 = memTuple->findColumn1(95)->tupleColumn();
    BatchLab::ITupleColumn* col_96 = memTuple->findColumn1(96)->tupleColumn();
    BatchLab::ITupleColumn* col_97 = memTuple->findColumn1(97)->tupleColumn();
    BatchLab::ITupleColumn* col_98 = memTuple->findColumn1(98)->tupleColumn();
    BatchLab::ITupleColumn* col_99 = memTuple->findColumn1(99)->tupleColumn();
    BatchLab::ITupleColumn* col_100 = memTuple->findColumn1(100)->tupleColumn();
    BatchLab::ITupleColumn* col_101 = memTuple->findColumn1(101)->tupleColumn();
    BatchLab::ITupleColumn* col_102 = memTuple->findColumn1(102)->tupleColumn();
    BatchLab::ITupleColumn* col_103 = memTuple->findColumn1(103)->tupleColumn();
    BatchLab::ITupleColumn* col_104 = memTuple->findColumn1(104)->tupleColumn();
    BatchLab::ITupleColumn* col_105 = memTuple->findColumn1(105)->tupleColumn();
    BatchLab::ITupleColumn* col_106 = memTuple->findColumn1(106)->tupleColumn();
    BatchLab::ITupleColumn* col_107 = memTuple->findColumn1(107)->tupleColumn();
    BatchLab::ITupleColumn* col_108 = memTuple->findColumn1(108)->tupleColumn();
    BatchLab::ITupleColumn* col_109 = memTuple->findColumn1(109)->tupleColumn();
    BatchLab::ITupleColumn* col_110 = memTuple->findColumn1(110)->tupleColumn();
    BatchLab::ITupleColumn* col_111 = memTuple->findColumn1(111)->tupleColumn();
    BatchLab::ITupleColumn* col_112 = memTuple->findColumn1(112)->tupleColumn();
    BatchLab::ITupleColumn* col_113 = memTuple->findColumn1(113)->tupleColumn();
    BatchLab::ITupleColumn* col_114 = memTuple->findColumn1(114)->tupleColumn();
    BatchLab::ITupleColumn* col_115 = memTuple->findColumn1(115)->tupleColumn();
    BatchLab::ITupleColumn* col_116 = memTuple->findColumn1(116)->tupleColumn();
    BatchLab::ITupleColumn* col_117 = memTuple->findColumn1(117)->tupleColumn();
    BatchLab::ITupleColumn* col_118 = memTuple->findColumn1(118)->tupleColumn();
    BatchLab::ITupleColumn* col_119 = memTuple->findColumn1(119)->tupleColumn();
    BatchLab::ITupleColumn* col_120 = memTuple->findColumn1(120)->tupleColumn();
    BatchLab::ITupleColumn* col_121 = memTuple->findColumn1(121)->tupleColumn();
    BatchLab::ITupleColumn* col_122 = memTuple->findColumn1(122)->tupleColumn();
    BatchLab::ITupleColumn* col_123 = memTuple->findColumn1(123)->tupleColumn();
    BatchLab::ITupleColumn* col_124 = memTuple->findColumn1(124)->tupleColumn();
    BatchLab::ITupleColumn* col_125 = memTuple->findColumn1(125)->tupleColumn();
    BatchLab::ITupleColumn* col_126 = memTuple->findColumn1(126)->tupleColumn();
    BatchLab::ITupleColumn* col_127 = memTuple->findColumn1(127)->tupleColumn();
    BatchLab::ITupleColumn* col_128 = memTuple->findColumn1(128)->tupleColumn();
    BatchLab::ITupleColumn* col_129 = memTuple->findColumn1(129)->tupleColumn();
    BatchLab::ITupleColumn* col_130 = memTuple->findColumn1(130)->tupleColumn();
    BatchLab::ITupleColumn* col_131 = memTuple->findColumn1(131)->tupleColumn();
    BatchLab::ITupleColumn* col_132 = memTuple->findColumn1(132)->tupleColumn();
    BatchLab::ITupleColumn* col_133 = memTuple->findColumn1(133)->tupleColumn();
    BatchLab::ITupleColumn* col_134 = memTuple->findColumn1(134)->tupleColumn();
    BatchLab::ITupleColumn* col_135 = memTuple->findColumn1(135)->tupleColumn();
    BatchLab::ITupleColumn* col_136 = memTuple->findColumn1(136)->tupleColumn();
    BatchLab::ITupleColumn* col_137 = memTuple->findColumn1(137)->tupleColumn();
    BatchLab::ITupleColumn* col_138 = memTuple->findColumn1(138)->tupleColumn();
    BatchLab::ITupleColumn* col_139 = memTuple->findColumn1(139)->tupleColumn();
    BatchLab::ITupleColumn* col_140 = memTuple->findColumn1(140)->tupleColumn();
    BatchLab::ITupleColumn* col_141 = memTuple->findColumn1(141)->tupleColumn();
    BatchLab::ITupleColumn* col_142 = memTuple->findColumn1(142)->tupleColumn();
    BatchLab::ITupleColumn* col_143 = memTuple->findColumn1(143)->tupleColumn();
    BatchLab::ITupleColumn* col_144 = memTuple->findColumn1(144)->tupleColumn();
    BatchLab::ITupleColumn* col_145 = memTuple->findColumn1(145)->tupleColumn();
    BatchLab::ITupleColumn* col_146 = memTuple->findColumn1(146)->tupleColumn();
    BatchLab::ITupleColumn* col_147 = memTuple->findColumn1(147)->tupleColumn();
    BatchLab::ITupleColumn* col_148 = memTuple->findColumn1(148)->tupleColumn();
    BatchLab::ITupleColumn* col_149 = memTuple->findColumn1(149)->tupleColumn();
    BatchLab::ITupleColumn* col_150 = memTuple->findColumn1(150)->tupleColumn();
    BatchLab::ITupleColumn* col_151 = memTuple->findColumn1(151)->tupleColumn();
    BatchLab::ITupleColumn* col_152 = memTuple->findColumn1(152)->tupleColumn();
    BatchLab::ITupleColumn* col_153 = memTuple->findColumn1(153)->tupleColumn();
    BatchLab::ITupleColumn* col_154 = memTuple->findColumn1(154)->tupleColumn();
    BatchLab::ITupleColumn* col_155 = memTuple->findColumn1(155)->tupleColumn();
    BatchLab::ITupleColumn* col_156 = memTuple->findColumn1(156)->tupleColumn();
    BatchLab::ITupleColumn* col_157 = memTuple->findColumn1(157)->tupleColumn();
    BatchLab::ITupleColumn* col_158 = memTuple->findColumn1(158)->tupleColumn();
    BatchLab::ITupleColumn* col_159 = memTuple->findColumn1(159)->tupleColumn();
    BatchLab::ITupleColumn* col_160 = memTuple->findColumn1(160)->tupleColumn();
    BatchLab::ITupleColumn* col_161 = memTuple->findColumn1(161)->tupleColumn();
    BatchLab::ITupleColumn* col_162 = memTuple->findColumn1(162)->tupleColumn();
    BatchLab::ITupleColumn* col_163 = memTuple->findColumn1(163)->tupleColumn();
    BatchLab::ITupleColumn* col_164 = memTuple->findColumn1(164)->tupleColumn();
    BatchLab::ITupleColumn* col_165 = memTuple->findColumn1(165)->tupleColumn();
    BatchLab::ITupleColumn* col_166 = memTuple->findColumn1(166)->tupleColumn();
    BatchLab::ITupleColumn* col_167 = memTuple->findColumn1(167)->tupleColumn();
    BatchLab::ITupleColumn* col_168 = memTuple->findColumn1(168)->tupleColumn();
    BatchLab::ITupleColumn* col_169 = memTuple->findColumn1(169)->tupleColumn();
    BatchLab::ITupleColumn* col_170 = memTuple->findColumn1(170)->tupleColumn();
    BatchLab::ITupleColumn* col_171 = memTuple->findColumn1(171)->tupleColumn();
    BatchLab::ITupleColumn* col_172 = memTuple->findColumn1(172)->tupleColumn();
    BatchLab::ITupleColumn* col_173 = memTuple->findColumn1(173)->tupleColumn();
    BatchLab::ITupleColumn* col_174 = memTuple->findColumn1(174)->tupleColumn();
    BatchLab::ITupleColumn* col_175 = memTuple->findColumn1(175)->tupleColumn();
    BatchLab::ITupleColumn* col_176 = memTuple->findColumn1(176)->tupleColumn();
    BatchLab::ITupleColumn* col_177 = memTuple->findColumn1(177)->tupleColumn();
    BatchLab::ITupleColumn* col_178 = memTuple->findColumn1(178)->tupleColumn();
    BatchLab::ITupleColumn* col_179 = memTuple->findColumn1(179)->tupleColumn();
    BatchLab::ITupleColumn* col_180 = memTuple->findColumn1(180)->tupleColumn();
    BatchLab::ITupleColumn* col_181 = memTuple->findColumn1(181)->tupleColumn();
    BatchLab::ITupleColumn* col_182 = memTuple->findColumn1(182)->tupleColumn();
    BatchLab::ITupleColumn* col_183 = memTuple->findColumn1(183)->tupleColumn();
    BatchLab::ITupleColumn* col_184 = memTuple->findColumn1(184)->tupleColumn();
    BatchLab::ITupleColumn* col_185 = memTuple->findColumn1(185)->tupleColumn();
    BatchLab::ITupleColumn* col_186 = memTuple->findColumn1(186)->tupleColumn();
    BatchLab::ITupleColumn* col_187 = memTuple->findColumn1(187)->tupleColumn();
    BatchLab::ITupleColumn* col_188 = memTuple->findColumn1(188)->tupleColumn();
    BatchLab::ITupleColumn* col_189 = memTuple->findColumn1(189)->tupleColumn();
    BatchLab::ITupleColumn* col_190 = memTuple->findColumn1(190)->tupleColumn();
    BatchLab::ITupleColumn* col_191 = memTuple->findColumn1(191)->tupleColumn();
    BatchLab::ITupleColumn* col_192 = memTuple->findColumn1(192)->tupleColumn();
    BatchLab::ITupleColumn* col_193 = memTuple->findColumn1(193)->tupleColumn();
    BatchLab::ITupleColumn* col_194 = memTuple->findColumn1(194)->tupleColumn();
    BatchLab::ITupleColumn* col_195 = memTuple->findColumn1(195)->tupleColumn();
    BatchLab::ITupleColumn* col_196 = memTuple->findColumn1(196)->tupleColumn();
    BatchLab::ITupleColumn* col_197 = memTuple->findColumn1(197)->tupleColumn();
    BatchLab::ITupleColumn* col_198 = memTuple->findColumn1(198)->tupleColumn();
    BatchLab::ITupleColumn* col_199 = memTuple->findColumn1(199)->tupleColumn();

    printf("debug : rows %d : %d %d\n",rown,nints,nfloats);
    Lib::Time start = Lib::Time::now();
    for(count=0;count<rown;count++) {
      col_0->fill((int)314);
      col_1->fill((int)314);
      col_2->fill((int)314);
      col_3->fill((int)314);
      col_4->fill((int)314);
      col_5->fill((int)314);
      col_6->fill((int)314);
      col_7->fill((int)314);
      col_8->fill((int)314);
      col_9->fill((int)314);
      col_10->fill((int)314);
      col_11->fill((int)314);
      col_12->fill((int)314);
      col_13->fill((int)314);
      col_14->fill((int)314);
      col_15->fill((int)314);
      col_16->fill((int)314);
      col_17->fill((int)314);
      col_18->fill((int)314);
      col_19->fill((int)314);
      col_20->fill((int)314);
      col_21->fill((int)314);
      col_22->fill((int)314);
      col_23->fill((int)314);
      col_24->fill((int)314);
      col_25->fill((int)314);
      col_26->fill((int)314);
      col_27->fill((int)314);
      col_28->fill((int)314);
      col_29->fill((int)314);
      col_30->fill((int)314);
      col_31->fill((int)314);
      col_32->fill((int)314);
      col_33->fill((int)314);
      col_34->fill((int)314);
      col_35->fill((int)314);
      col_36->fill((int)314);
      col_37->fill((int)314);
      col_38->fill((int)314);
      col_39->fill((int)314);
      col_40->fill((int)314);
      col_41->fill((int)314);
      col_42->fill((int)314);
      col_43->fill((int)314);
      col_44->fill((int)314);
      col_45->fill((int)314);
      col_46->fill((int)314);
      col_47->fill((int)314);
      col_48->fill((int)314);
      col_49->fill((int)314);
      col_50->fill((int)314);
      col_51->fill((int)314);
      col_52->fill((int)314);
      col_53->fill((int)314);
      col_54->fill((int)314);
      col_55->fill((int)314);
      col_56->fill((int)314);
      col_57->fill((int)314);
      col_58->fill((int)314);
      col_59->fill((int)314);
      col_60->fill((int)314);
      col_61->fill((int)314);
      col_62->fill((int)314);
      col_63->fill((int)314);
      col_64->fill((int)314);
      col_65->fill((int)314);
      col_66->fill((int)314);
      col_67->fill((int)314);
      col_68->fill((int)314);
      col_69->fill((int)314);
      col_70->fill((int)314);
      col_71->fill((int)314);
      col_72->fill((int)314);
      col_73->fill((int)314);
      col_74->fill((int)314);
      col_75->fill((int)314);
      col_76->fill((int)314);
      col_77->fill((int)314);
      col_78->fill((int)314);
      col_79->fill((int)314);
      col_80->fill((int)314);
      col_81->fill((int)314);
      col_82->fill((int)314);
      col_83->fill((int)314);
      col_84->fill((int)314);
      col_85->fill((int)314);
      col_86->fill((int)314);
      col_87->fill((int)314);
      col_88->fill((int)314);
      col_89->fill((int)314);
      col_90->fill((int)314);
      col_91->fill((int)314);
      col_92->fill((int)314);
      col_93->fill((int)314);
      col_94->fill((int)314);
      col_95->fill((int)314);
      col_96->fill((int)314);
      col_97->fill((int)314);
      col_98->fill((int)314);
      col_99->fill((int)314);
      col_100->fill((float)314);
      col_101->fill((float)314);
      col_102->fill((float)314);
      col_103->fill((float)314);
      col_104->fill((float)314);
      col_105->fill((float)314);
      col_106->fill((float)314);
      col_107->fill((float)314);
      col_108->fill((float)314);
      col_109->fill((float)314);
      col_110->fill((float)314);
      col_111->fill((float)314);
      col_112->fill((float)314);
      col_113->fill((float)314);
      col_114->fill((float)314);
      col_115->fill((float)314);
      col_116->fill((float)314);
      col_117->fill((float)314);
      col_118->fill((float)314);
      col_119->fill((float)314);
      col_120->fill((float)314);
      col_121->fill((float)314);
      col_122->fill((float)314);
      col_123->fill((float)314);
      col_124->fill((float)314);
      col_125->fill((float)314);
      col_126->fill((float)314);
      col_127->fill((float)314);
      col_128->fill((float)314);
      col_129->fill((float)314);
      col_130->fill((float)314);
      col_131->fill((float)314);
      col_132->fill((float)314);
      col_133->fill((float)314);
      col_134->fill((float)314);
      col_135->fill((float)314);
      col_136->fill((float)314);
      col_137->fill((float)314);
      col_138->fill((float)314);
      col_139->fill((float)314);
      col_140->fill((float)314);
      col_141->fill((float)314);
      col_142->fill((float)314);
      col_143->fill((float)314);
      col_144->fill((float)314);
      col_145->fill((float)314);
      col_146->fill((float)314);
      col_147->fill((float)314);
      col_148->fill((float)314);
      col_149->fill((float)314);
      col_150->fill((float)314);
      col_151->fill((float)314);
      col_152->fill((float)314);
      col_153->fill((float)314);
      col_154->fill((float)314);
      col_155->fill((float)314);
      col_156->fill((float)314);
      col_157->fill((float)314);
      col_158->fill((float)314);
      col_159->fill((float)314);
      col_160->fill((float)314);
      col_161->fill((float)314);
      col_162->fill((float)314);
      col_163->fill((float)314);
      col_164->fill((float)314);
      col_165->fill((float)314);
      col_166->fill((float)314);
      col_167->fill((float)314);
      col_168->fill((float)314);
      col_169->fill((float)314);
      col_170->fill((float)314);
      col_171->fill((float)314);
      col_172->fill((float)314);
      col_173->fill((float)314);
      col_174->fill((float)314);
      col_175->fill((float)314);
      col_176->fill((float)314);
      col_177->fill((float)314);
      col_178->fill((float)314);
      col_179->fill((float)314);
      col_180->fill((float)314);
      col_181->fill((float)314);
      col_182->fill((float)314);
      col_183->fill((float)314);
      col_184->fill((float)314);
      col_185->fill((float)314);
      col_186->fill((float)314);
      col_187->fill((float)314);
      col_188->fill((float)314);
      col_189->fill((float)314);
      col_190->fill((float)314);
      col_191->fill((float)314);
      col_192->fill((float)314);
      col_193->fill((float)314);
      col_194->fill((float)314);
      col_195->fill((float)314);
      col_196->fill((float)314);
      col_197->fill((float)314);
      col_198->fill((float)314);
      col_199->fill((float)314);

      tuple->addRow();
    }
    Lib::Time elapsed = Lib::Time::elapsed(start);
    printf("debug : Time : %g\n",elapsed.seconds());
  
  } else if(test=="tc1") {

    BatchLab::MemoryTuple* memTuple =  INLIB_CAST(*tuple,BatchLab::MemoryTuple);

    std::vector<BatchLab::ITupleColumn*> baseCols;
    for(icol=0;icol<nints;icol++) {
      BatchLab::BaseColumn* baseCol = memTuple->findColumn1(icol);
      baseCols.push_back(baseCol->tupleColumn());
    }  
    for(icol=0;icol<nfloats;icol++) {
      BatchLab::BaseColumn* baseCol = memTuple->findColumn1(nints+icol);
      baseCols.push_back(baseCol->tupleColumn());
    }  


    printf("debug : rows %d : %d %d\n",rown,nints,nfloats);
    Lib::Time start = Lib::Time::now();
    for(count=0;count<rown;count++) {
      for(icol=0;icol<nints;icol++) {
        BatchLab::ITupleColumn* baseCol = baseCols[icol];
        baseCol->fill(314);
      }
      for(icol=0;icol<nfloats;icol++) {
        BatchLab::ITupleColumn* baseCol = baseCols[nints+icol];
        baseCol->fill(314);
      }
      tuple->addRow();
    }
    Lib::Time elapsed = Lib::Time::elapsed(start);
    printf("debug : Time : %g\n",elapsed.seconds());
  
  } else if(test=="bind") {

    BatchLab::MemoryTuple* memTuple =  INLIB_CAST(*tuple,BatchLab::MemoryTuple);

    BatchLab::ITupleColumn* col_0 = memTuple->findColumn1(0)->tupleColumn();
    BatchLab::ITupleColumn* col_1 = memTuple->findColumn1(1)->tupleColumn();
    BatchLab::ITupleColumn* col_2 = memTuple->findColumn1(2)->tupleColumn();
    BatchLab::ITupleColumn* col_3 = memTuple->findColumn1(3)->tupleColumn();
    BatchLab::ITupleColumn* col_4 = memTuple->findColumn1(4)->tupleColumn();
    BatchLab::ITupleColumn* col_5 = memTuple->findColumn1(5)->tupleColumn();
    BatchLab::ITupleColumn* col_6 = memTuple->findColumn1(6)->tupleColumn();
    BatchLab::ITupleColumn* col_7 = memTuple->findColumn1(7)->tupleColumn();
    BatchLab::ITupleColumn* col_8 = memTuple->findColumn1(8)->tupleColumn();
    BatchLab::ITupleColumn* col_9 = memTuple->findColumn1(9)->tupleColumn();
    BatchLab::ITupleColumn* col_10 = memTuple->findColumn1(10)->tupleColumn();
    BatchLab::ITupleColumn* col_11 = memTuple->findColumn1(11)->tupleColumn();
    BatchLab::ITupleColumn* col_12 = memTuple->findColumn1(12)->tupleColumn();
    BatchLab::ITupleColumn* col_13 = memTuple->findColumn1(13)->tupleColumn();
    BatchLab::ITupleColumn* col_14 = memTuple->findColumn1(14)->tupleColumn();
    BatchLab::ITupleColumn* col_15 = memTuple->findColumn1(15)->tupleColumn();
    BatchLab::ITupleColumn* col_16 = memTuple->findColumn1(16)->tupleColumn();
    BatchLab::ITupleColumn* col_17 = memTuple->findColumn1(17)->tupleColumn();
    BatchLab::ITupleColumn* col_18 = memTuple->findColumn1(18)->tupleColumn();
    BatchLab::ITupleColumn* col_19 = memTuple->findColumn1(19)->tupleColumn();
    BatchLab::ITupleColumn* col_20 = memTuple->findColumn1(20)->tupleColumn();
    BatchLab::ITupleColumn* col_21 = memTuple->findColumn1(21)->tupleColumn();
    BatchLab::ITupleColumn* col_22 = memTuple->findColumn1(22)->tupleColumn();
    BatchLab::ITupleColumn* col_23 = memTuple->findColumn1(23)->tupleColumn();
    BatchLab::ITupleColumn* col_24 = memTuple->findColumn1(24)->tupleColumn();
    BatchLab::ITupleColumn* col_25 = memTuple->findColumn1(25)->tupleColumn();
    BatchLab::ITupleColumn* col_26 = memTuple->findColumn1(26)->tupleColumn();
    BatchLab::ITupleColumn* col_27 = memTuple->findColumn1(27)->tupleColumn();
    BatchLab::ITupleColumn* col_28 = memTuple->findColumn1(28)->tupleColumn();
    BatchLab::ITupleColumn* col_29 = memTuple->findColumn1(29)->tupleColumn();
    BatchLab::ITupleColumn* col_30 = memTuple->findColumn1(30)->tupleColumn();
    BatchLab::ITupleColumn* col_31 = memTuple->findColumn1(31)->tupleColumn();
    BatchLab::ITupleColumn* col_32 = memTuple->findColumn1(32)->tupleColumn();
    BatchLab::ITupleColumn* col_33 = memTuple->findColumn1(33)->tupleColumn();
    BatchLab::ITupleColumn* col_34 = memTuple->findColumn1(34)->tupleColumn();
    BatchLab::ITupleColumn* col_35 = memTuple->findColumn1(35)->tupleColumn();
    BatchLab::ITupleColumn* col_36 = memTuple->findColumn1(36)->tupleColumn();
    BatchLab::ITupleColumn* col_37 = memTuple->findColumn1(37)->tupleColumn();
    BatchLab::ITupleColumn* col_38 = memTuple->findColumn1(38)->tupleColumn();
    BatchLab::ITupleColumn* col_39 = memTuple->findColumn1(39)->tupleColumn();
    BatchLab::ITupleColumn* col_40 = memTuple->findColumn1(40)->tupleColumn();
    BatchLab::ITupleColumn* col_41 = memTuple->findColumn1(41)->tupleColumn();
    BatchLab::ITupleColumn* col_42 = memTuple->findColumn1(42)->tupleColumn();
    BatchLab::ITupleColumn* col_43 = memTuple->findColumn1(43)->tupleColumn();
    BatchLab::ITupleColumn* col_44 = memTuple->findColumn1(44)->tupleColumn();
    BatchLab::ITupleColumn* col_45 = memTuple->findColumn1(45)->tupleColumn();
    BatchLab::ITupleColumn* col_46 = memTuple->findColumn1(46)->tupleColumn();
    BatchLab::ITupleColumn* col_47 = memTuple->findColumn1(47)->tupleColumn();
    BatchLab::ITupleColumn* col_48 = memTuple->findColumn1(48)->tupleColumn();
    BatchLab::ITupleColumn* col_49 = memTuple->findColumn1(49)->tupleColumn();
    BatchLab::ITupleColumn* col_50 = memTuple->findColumn1(50)->tupleColumn();
    BatchLab::ITupleColumn* col_51 = memTuple->findColumn1(51)->tupleColumn();
    BatchLab::ITupleColumn* col_52 = memTuple->findColumn1(52)->tupleColumn();
    BatchLab::ITupleColumn* col_53 = memTuple->findColumn1(53)->tupleColumn();
    BatchLab::ITupleColumn* col_54 = memTuple->findColumn1(54)->tupleColumn();
    BatchLab::ITupleColumn* col_55 = memTuple->findColumn1(55)->tupleColumn();
    BatchLab::ITupleColumn* col_56 = memTuple->findColumn1(56)->tupleColumn();
    BatchLab::ITupleColumn* col_57 = memTuple->findColumn1(57)->tupleColumn();
    BatchLab::ITupleColumn* col_58 = memTuple->findColumn1(58)->tupleColumn();
    BatchLab::ITupleColumn* col_59 = memTuple->findColumn1(59)->tupleColumn();
    BatchLab::ITupleColumn* col_60 = memTuple->findColumn1(60)->tupleColumn();
    BatchLab::ITupleColumn* col_61 = memTuple->findColumn1(61)->tupleColumn();
    BatchLab::ITupleColumn* col_62 = memTuple->findColumn1(62)->tupleColumn();
    BatchLab::ITupleColumn* col_63 = memTuple->findColumn1(63)->tupleColumn();
    BatchLab::ITupleColumn* col_64 = memTuple->findColumn1(64)->tupleColumn();
    BatchLab::ITupleColumn* col_65 = memTuple->findColumn1(65)->tupleColumn();
    BatchLab::ITupleColumn* col_66 = memTuple->findColumn1(66)->tupleColumn();
    BatchLab::ITupleColumn* col_67 = memTuple->findColumn1(67)->tupleColumn();
    BatchLab::ITupleColumn* col_68 = memTuple->findColumn1(68)->tupleColumn();
    BatchLab::ITupleColumn* col_69 = memTuple->findColumn1(69)->tupleColumn();
    BatchLab::ITupleColumn* col_70 = memTuple->findColumn1(70)->tupleColumn();
    BatchLab::ITupleColumn* col_71 = memTuple->findColumn1(71)->tupleColumn();
    BatchLab::ITupleColumn* col_72 = memTuple->findColumn1(72)->tupleColumn();
    BatchLab::ITupleColumn* col_73 = memTuple->findColumn1(73)->tupleColumn();
    BatchLab::ITupleColumn* col_74 = memTuple->findColumn1(74)->tupleColumn();
    BatchLab::ITupleColumn* col_75 = memTuple->findColumn1(75)->tupleColumn();
    BatchLab::ITupleColumn* col_76 = memTuple->findColumn1(76)->tupleColumn();
    BatchLab::ITupleColumn* col_77 = memTuple->findColumn1(77)->tupleColumn();
    BatchLab::ITupleColumn* col_78 = memTuple->findColumn1(78)->tupleColumn();
    BatchLab::ITupleColumn* col_79 = memTuple->findColumn1(79)->tupleColumn();
    BatchLab::ITupleColumn* col_80 = memTuple->findColumn1(80)->tupleColumn();
    BatchLab::ITupleColumn* col_81 = memTuple->findColumn1(81)->tupleColumn();
    BatchLab::ITupleColumn* col_82 = memTuple->findColumn1(82)->tupleColumn();
    BatchLab::ITupleColumn* col_83 = memTuple->findColumn1(83)->tupleColumn();
    BatchLab::ITupleColumn* col_84 = memTuple->findColumn1(84)->tupleColumn();
    BatchLab::ITupleColumn* col_85 = memTuple->findColumn1(85)->tupleColumn();
    BatchLab::ITupleColumn* col_86 = memTuple->findColumn1(86)->tupleColumn();
    BatchLab::ITupleColumn* col_87 = memTuple->findColumn1(87)->tupleColumn();
    BatchLab::ITupleColumn* col_88 = memTuple->findColumn1(88)->tupleColumn();
    BatchLab::ITupleColumn* col_89 = memTuple->findColumn1(89)->tupleColumn();
    BatchLab::ITupleColumn* col_90 = memTuple->findColumn1(90)->tupleColumn();
    BatchLab::ITupleColumn* col_91 = memTuple->findColumn1(91)->tupleColumn();
    BatchLab::ITupleColumn* col_92 = memTuple->findColumn1(92)->tupleColumn();
    BatchLab::ITupleColumn* col_93 = memTuple->findColumn1(93)->tupleColumn();
    BatchLab::ITupleColumn* col_94 = memTuple->findColumn1(94)->tupleColumn();
    BatchLab::ITupleColumn* col_95 = memTuple->findColumn1(95)->tupleColumn();
    BatchLab::ITupleColumn* col_96 = memTuple->findColumn1(96)->tupleColumn();
    BatchLab::ITupleColumn* col_97 = memTuple->findColumn1(97)->tupleColumn();
    BatchLab::ITupleColumn* col_98 = memTuple->findColumn1(98)->tupleColumn();
    BatchLab::ITupleColumn* col_99 = memTuple->findColumn1(99)->tupleColumn();
    BatchLab::ITupleColumn* col_100 = memTuple->findColumn1(100)->tupleColumn();
    BatchLab::ITupleColumn* col_101 = memTuple->findColumn1(101)->tupleColumn();
    BatchLab::ITupleColumn* col_102 = memTuple->findColumn1(102)->tupleColumn();
    BatchLab::ITupleColumn* col_103 = memTuple->findColumn1(103)->tupleColumn();
    BatchLab::ITupleColumn* col_104 = memTuple->findColumn1(104)->tupleColumn();
    BatchLab::ITupleColumn* col_105 = memTuple->findColumn1(105)->tupleColumn();
    BatchLab::ITupleColumn* col_106 = memTuple->findColumn1(106)->tupleColumn();
    BatchLab::ITupleColumn* col_107 = memTuple->findColumn1(107)->tupleColumn();
    BatchLab::ITupleColumn* col_108 = memTuple->findColumn1(108)->tupleColumn();
    BatchLab::ITupleColumn* col_109 = memTuple->findColumn1(109)->tupleColumn();
    BatchLab::ITupleColumn* col_110 = memTuple->findColumn1(110)->tupleColumn();
    BatchLab::ITupleColumn* col_111 = memTuple->findColumn1(111)->tupleColumn();
    BatchLab::ITupleColumn* col_112 = memTuple->findColumn1(112)->tupleColumn();
    BatchLab::ITupleColumn* col_113 = memTuple->findColumn1(113)->tupleColumn();
    BatchLab::ITupleColumn* col_114 = memTuple->findColumn1(114)->tupleColumn();
    BatchLab::ITupleColumn* col_115 = memTuple->findColumn1(115)->tupleColumn();
    BatchLab::ITupleColumn* col_116 = memTuple->findColumn1(116)->tupleColumn();
    BatchLab::ITupleColumn* col_117 = memTuple->findColumn1(117)->tupleColumn();
    BatchLab::ITupleColumn* col_118 = memTuple->findColumn1(118)->tupleColumn();
    BatchLab::ITupleColumn* col_119 = memTuple->findColumn1(119)->tupleColumn();
    BatchLab::ITupleColumn* col_120 = memTuple->findColumn1(120)->tupleColumn();
    BatchLab::ITupleColumn* col_121 = memTuple->findColumn1(121)->tupleColumn();
    BatchLab::ITupleColumn* col_122 = memTuple->findColumn1(122)->tupleColumn();
    BatchLab::ITupleColumn* col_123 = memTuple->findColumn1(123)->tupleColumn();
    BatchLab::ITupleColumn* col_124 = memTuple->findColumn1(124)->tupleColumn();
    BatchLab::ITupleColumn* col_125 = memTuple->findColumn1(125)->tupleColumn();
    BatchLab::ITupleColumn* col_126 = memTuple->findColumn1(126)->tupleColumn();
    BatchLab::ITupleColumn* col_127 = memTuple->findColumn1(127)->tupleColumn();
    BatchLab::ITupleColumn* col_128 = memTuple->findColumn1(128)->tupleColumn();
    BatchLab::ITupleColumn* col_129 = memTuple->findColumn1(129)->tupleColumn();
    BatchLab::ITupleColumn* col_130 = memTuple->findColumn1(130)->tupleColumn();
    BatchLab::ITupleColumn* col_131 = memTuple->findColumn1(131)->tupleColumn();
    BatchLab::ITupleColumn* col_132 = memTuple->findColumn1(132)->tupleColumn();
    BatchLab::ITupleColumn* col_133 = memTuple->findColumn1(133)->tupleColumn();
    BatchLab::ITupleColumn* col_134 = memTuple->findColumn1(134)->tupleColumn();
    BatchLab::ITupleColumn* col_135 = memTuple->findColumn1(135)->tupleColumn();
    BatchLab::ITupleColumn* col_136 = memTuple->findColumn1(136)->tupleColumn();
    BatchLab::ITupleColumn* col_137 = memTuple->findColumn1(137)->tupleColumn();
    BatchLab::ITupleColumn* col_138 = memTuple->findColumn1(138)->tupleColumn();
    BatchLab::ITupleColumn* col_139 = memTuple->findColumn1(139)->tupleColumn();
    BatchLab::ITupleColumn* col_140 = memTuple->findColumn1(140)->tupleColumn();
    BatchLab::ITupleColumn* col_141 = memTuple->findColumn1(141)->tupleColumn();
    BatchLab::ITupleColumn* col_142 = memTuple->findColumn1(142)->tupleColumn();
    BatchLab::ITupleColumn* col_143 = memTuple->findColumn1(143)->tupleColumn();
    BatchLab::ITupleColumn* col_144 = memTuple->findColumn1(144)->tupleColumn();
    BatchLab::ITupleColumn* col_145 = memTuple->findColumn1(145)->tupleColumn();
    BatchLab::ITupleColumn* col_146 = memTuple->findColumn1(146)->tupleColumn();
    BatchLab::ITupleColumn* col_147 = memTuple->findColumn1(147)->tupleColumn();
    BatchLab::ITupleColumn* col_148 = memTuple->findColumn1(148)->tupleColumn();
    BatchLab::ITupleColumn* col_149 = memTuple->findColumn1(149)->tupleColumn();
    BatchLab::ITupleColumn* col_150 = memTuple->findColumn1(150)->tupleColumn();
    BatchLab::ITupleColumn* col_151 = memTuple->findColumn1(151)->tupleColumn();
    BatchLab::ITupleColumn* col_152 = memTuple->findColumn1(152)->tupleColumn();
    BatchLab::ITupleColumn* col_153 = memTuple->findColumn1(153)->tupleColumn();
    BatchLab::ITupleColumn* col_154 = memTuple->findColumn1(154)->tupleColumn();
    BatchLab::ITupleColumn* col_155 = memTuple->findColumn1(155)->tupleColumn();
    BatchLab::ITupleColumn* col_156 = memTuple->findColumn1(156)->tupleColumn();
    BatchLab::ITupleColumn* col_157 = memTuple->findColumn1(157)->tupleColumn();
    BatchLab::ITupleColumn* col_158 = memTuple->findColumn1(158)->tupleColumn();
    BatchLab::ITupleColumn* col_159 = memTuple->findColumn1(159)->tupleColumn();
    BatchLab::ITupleColumn* col_160 = memTuple->findColumn1(160)->tupleColumn();
    BatchLab::ITupleColumn* col_161 = memTuple->findColumn1(161)->tupleColumn();
    BatchLab::ITupleColumn* col_162 = memTuple->findColumn1(162)->tupleColumn();
    BatchLab::ITupleColumn* col_163 = memTuple->findColumn1(163)->tupleColumn();
    BatchLab::ITupleColumn* col_164 = memTuple->findColumn1(164)->tupleColumn();
    BatchLab::ITupleColumn* col_165 = memTuple->findColumn1(165)->tupleColumn();
    BatchLab::ITupleColumn* col_166 = memTuple->findColumn1(166)->tupleColumn();
    BatchLab::ITupleColumn* col_167 = memTuple->findColumn1(167)->tupleColumn();
    BatchLab::ITupleColumn* col_168 = memTuple->findColumn1(168)->tupleColumn();
    BatchLab::ITupleColumn* col_169 = memTuple->findColumn1(169)->tupleColumn();
    BatchLab::ITupleColumn* col_170 = memTuple->findColumn1(170)->tupleColumn();
    BatchLab::ITupleColumn* col_171 = memTuple->findColumn1(171)->tupleColumn();
    BatchLab::ITupleColumn* col_172 = memTuple->findColumn1(172)->tupleColumn();
    BatchLab::ITupleColumn* col_173 = memTuple->findColumn1(173)->tupleColumn();
    BatchLab::ITupleColumn* col_174 = memTuple->findColumn1(174)->tupleColumn();
    BatchLab::ITupleColumn* col_175 = memTuple->findColumn1(175)->tupleColumn();
    BatchLab::ITupleColumn* col_176 = memTuple->findColumn1(176)->tupleColumn();
    BatchLab::ITupleColumn* col_177 = memTuple->findColumn1(177)->tupleColumn();
    BatchLab::ITupleColumn* col_178 = memTuple->findColumn1(178)->tupleColumn();
    BatchLab::ITupleColumn* col_179 = memTuple->findColumn1(179)->tupleColumn();
    BatchLab::ITupleColumn* col_180 = memTuple->findColumn1(180)->tupleColumn();
    BatchLab::ITupleColumn* col_181 = memTuple->findColumn1(181)->tupleColumn();
    BatchLab::ITupleColumn* col_182 = memTuple->findColumn1(182)->tupleColumn();
    BatchLab::ITupleColumn* col_183 = memTuple->findColumn1(183)->tupleColumn();
    BatchLab::ITupleColumn* col_184 = memTuple->findColumn1(184)->tupleColumn();
    BatchLab::ITupleColumn* col_185 = memTuple->findColumn1(185)->tupleColumn();
    BatchLab::ITupleColumn* col_186 = memTuple->findColumn1(186)->tupleColumn();
    BatchLab::ITupleColumn* col_187 = memTuple->findColumn1(187)->tupleColumn();
    BatchLab::ITupleColumn* col_188 = memTuple->findColumn1(188)->tupleColumn();
    BatchLab::ITupleColumn* col_189 = memTuple->findColumn1(189)->tupleColumn();
    BatchLab::ITupleColumn* col_190 = memTuple->findColumn1(190)->tupleColumn();
    BatchLab::ITupleColumn* col_191 = memTuple->findColumn1(191)->tupleColumn();
    BatchLab::ITupleColumn* col_192 = memTuple->findColumn1(192)->tupleColumn();
    BatchLab::ITupleColumn* col_193 = memTuple->findColumn1(193)->tupleColumn();
    BatchLab::ITupleColumn* col_194 = memTuple->findColumn1(194)->tupleColumn();
    BatchLab::ITupleColumn* col_195 = memTuple->findColumn1(195)->tupleColumn();
    BatchLab::ITupleColumn* col_196 = memTuple->findColumn1(196)->tupleColumn();
    BatchLab::ITupleColumn* col_197 = memTuple->findColumn1(197)->tupleColumn();
    BatchLab::ITupleColumn* col_198 = memTuple->findColumn1(198)->tupleColumn();
    BatchLab::ITupleColumn* col_199 = memTuple->findColumn1(199)->tupleColumn();

    int int_variable = 0;
    float float_variable = 0;

    col_0->bindVariable(&int_variable);
    col_1->bindVariable(&int_variable);
    col_2->bindVariable(&int_variable);
    col_3->bindVariable(&int_variable);
    col_4->bindVariable(&int_variable);
    col_5->bindVariable(&int_variable);
    col_6->bindVariable(&int_variable);
    col_7->bindVariable(&int_variable);
    col_8->bindVariable(&int_variable);
    col_9->bindVariable(&int_variable);
    col_10->bindVariable(&int_variable);
    col_11->bindVariable(&int_variable);
    col_12->bindVariable(&int_variable);
    col_13->bindVariable(&int_variable);
    col_14->bindVariable(&int_variable);
    col_15->bindVariable(&int_variable);
    col_16->bindVariable(&int_variable);
    col_17->bindVariable(&int_variable);
    col_18->bindVariable(&int_variable);
    col_19->bindVariable(&int_variable);
    col_20->bindVariable(&int_variable);
    col_21->bindVariable(&int_variable);
    col_22->bindVariable(&int_variable);
    col_23->bindVariable(&int_variable);
    col_24->bindVariable(&int_variable);
    col_25->bindVariable(&int_variable);
    col_26->bindVariable(&int_variable);
    col_27->bindVariable(&int_variable);
    col_28->bindVariable(&int_variable);
    col_29->bindVariable(&int_variable);
    col_30->bindVariable(&int_variable);
    col_31->bindVariable(&int_variable);
    col_32->bindVariable(&int_variable);
    col_33->bindVariable(&int_variable);
    col_34->bindVariable(&int_variable);
    col_35->bindVariable(&int_variable);
    col_36->bindVariable(&int_variable);
    col_37->bindVariable(&int_variable);
    col_38->bindVariable(&int_variable);
    col_39->bindVariable(&int_variable);
    col_40->bindVariable(&int_variable);
    col_41->bindVariable(&int_variable);
    col_42->bindVariable(&int_variable);
    col_43->bindVariable(&int_variable);
    col_44->bindVariable(&int_variable);
    col_45->bindVariable(&int_variable);
    col_46->bindVariable(&int_variable);
    col_47->bindVariable(&int_variable);
    col_48->bindVariable(&int_variable);
    col_49->bindVariable(&int_variable);
    col_50->bindVariable(&int_variable);
    col_51->bindVariable(&int_variable);
    col_52->bindVariable(&int_variable);
    col_53->bindVariable(&int_variable);
    col_54->bindVariable(&int_variable);
    col_55->bindVariable(&int_variable);
    col_56->bindVariable(&int_variable);
    col_57->bindVariable(&int_variable);
    col_58->bindVariable(&int_variable);
    col_59->bindVariable(&int_variable);
    col_60->bindVariable(&int_variable);
    col_61->bindVariable(&int_variable);
    col_62->bindVariable(&int_variable);
    col_63->bindVariable(&int_variable);
    col_64->bindVariable(&int_variable);
    col_65->bindVariable(&int_variable);
    col_66->bindVariable(&int_variable);
    col_67->bindVariable(&int_variable);
    col_68->bindVariable(&int_variable);
    col_69->bindVariable(&int_variable);
    col_70->bindVariable(&int_variable);
    col_71->bindVariable(&int_variable);
    col_72->bindVariable(&int_variable);
    col_73->bindVariable(&int_variable);
    col_74->bindVariable(&int_variable);
    col_75->bindVariable(&int_variable);
    col_76->bindVariable(&int_variable);
    col_77->bindVariable(&int_variable);
    col_78->bindVariable(&int_variable);
    col_79->bindVariable(&int_variable);
    col_80->bindVariable(&int_variable);
    col_81->bindVariable(&int_variable);
    col_82->bindVariable(&int_variable);
    col_83->bindVariable(&int_variable);
    col_84->bindVariable(&int_variable);
    col_85->bindVariable(&int_variable);
    col_86->bindVariable(&int_variable);
    col_87->bindVariable(&int_variable);
    col_88->bindVariable(&int_variable);
    col_89->bindVariable(&int_variable);
    col_90->bindVariable(&int_variable);
    col_91->bindVariable(&int_variable);
    col_92->bindVariable(&int_variable);
    col_93->bindVariable(&int_variable);
    col_94->bindVariable(&int_variable);
    col_95->bindVariable(&int_variable);
    col_96->bindVariable(&int_variable);
    col_97->bindVariable(&int_variable);
    col_98->bindVariable(&int_variable);
    col_99->bindVariable(&int_variable);
    col_100->bindVariable(&float_variable);
    col_101->bindVariable(&float_variable);
    col_102->bindVariable(&float_variable);
    col_103->bindVariable(&float_variable);
    col_104->bindVariable(&float_variable);
    col_105->bindVariable(&float_variable);
    col_106->bindVariable(&float_variable);
    col_107->bindVariable(&float_variable);
    col_108->bindVariable(&float_variable);
    col_109->bindVariable(&float_variable);
    col_110->bindVariable(&float_variable);
    col_111->bindVariable(&float_variable);
    col_112->bindVariable(&float_variable);
    col_113->bindVariable(&float_variable);
    col_114->bindVariable(&float_variable);
    col_115->bindVariable(&float_variable);
    col_116->bindVariable(&float_variable);
    col_117->bindVariable(&float_variable);
    col_118->bindVariable(&float_variable);
    col_119->bindVariable(&float_variable);
    col_120->bindVariable(&float_variable);
    col_121->bindVariable(&float_variable);
    col_122->bindVariable(&float_variable);
    col_123->bindVariable(&float_variable);
    col_124->bindVariable(&float_variable);
    col_125->bindVariable(&float_variable);
    col_126->bindVariable(&float_variable);
    col_127->bindVariable(&float_variable);
    col_128->bindVariable(&float_variable);
    col_129->bindVariable(&float_variable);
    col_130->bindVariable(&float_variable);
    col_131->bindVariable(&float_variable);
    col_132->bindVariable(&float_variable);
    col_133->bindVariable(&float_variable);
    col_134->bindVariable(&float_variable);
    col_135->bindVariable(&float_variable);
    col_136->bindVariable(&float_variable);
    col_137->bindVariable(&float_variable);
    col_138->bindVariable(&float_variable);
    col_139->bindVariable(&float_variable);
    col_140->bindVariable(&float_variable);
    col_141->bindVariable(&float_variable);
    col_142->bindVariable(&float_variable);
    col_143->bindVariable(&float_variable);
    col_144->bindVariable(&float_variable);
    col_145->bindVariable(&float_variable);
    col_146->bindVariable(&float_variable);
    col_147->bindVariable(&float_variable);
    col_148->bindVariable(&float_variable);
    col_149->bindVariable(&float_variable);
    col_150->bindVariable(&float_variable);
    col_151->bindVariable(&float_variable);
    col_152->bindVariable(&float_variable);
    col_153->bindVariable(&float_variable);
    col_154->bindVariable(&float_variable);
    col_155->bindVariable(&float_variable);
    col_156->bindVariable(&float_variable);
    col_157->bindVariable(&float_variable);
    col_158->bindVariable(&float_variable);
    col_159->bindVariable(&float_variable);
    col_160->bindVariable(&float_variable);
    col_161->bindVariable(&float_variable);
    col_162->bindVariable(&float_variable);
    col_163->bindVariable(&float_variable);
    col_164->bindVariable(&float_variable);
    col_165->bindVariable(&float_variable);
    col_166->bindVariable(&float_variable);
    col_167->bindVariable(&float_variable);
    col_168->bindVariable(&float_variable);
    col_169->bindVariable(&float_variable);
    col_170->bindVariable(&float_variable);
    col_171->bindVariable(&float_variable);
    col_172->bindVariable(&float_variable);
    col_173->bindVariable(&float_variable);
    col_174->bindVariable(&float_variable);
    col_175->bindVariable(&float_variable);
    col_176->bindVariable(&float_variable);
    col_177->bindVariable(&float_variable);
    col_178->bindVariable(&float_variable);
    col_179->bindVariable(&float_variable);
    col_180->bindVariable(&float_variable);
    col_181->bindVariable(&float_variable);
    col_182->bindVariable(&float_variable);
    col_183->bindVariable(&float_variable);
    col_184->bindVariable(&float_variable);
    col_185->bindVariable(&float_variable);
    col_186->bindVariable(&float_variable);
    col_187->bindVariable(&float_variable);
    col_188->bindVariable(&float_variable);
    col_189->bindVariable(&float_variable);
    col_190->bindVariable(&float_variable);
    col_191->bindVariable(&float_variable);
    col_192->bindVariable(&float_variable);
    col_193->bindVariable(&float_variable);
    col_194->bindVariable(&float_variable);
    col_195->bindVariable(&float_variable);
    col_196->bindVariable(&float_variable);
    col_197->bindVariable(&float_variable);
    col_198->bindVariable(&float_variable);
    col_199->bindVariable(&float_variable);

    printf("debug : rows %d : %d %d\n",rown,nints,nfloats);
    Lib::Time start = Lib::Time::now();
    for(count=0;count<rown;count++) {
      int_variable = count;
      float_variable = count;

      tuple->addRow();
    }
    Lib::Time elapsed = Lib::Time::elapsed(start);
    printf("debug : Time : %g\n",elapsed.seconds());
  
  } else if(test=="aida") {

    printf("debug : rows %d : %d %d\n",rown,nints,nfloats);
    Lib::Time start = Lib::Time::now();
    for(count=0;count<rown;count++) {
      for(icol=0;icol<nints;icol++) {
        tuple->fill(icol,314);
      }
      for(icol=0;icol<nfloats;icol++) {
        tuple->fill(nints+icol,(float)314);
      }
      tuple->addRow();
    }
    Lib::Time elapsed = Lib::Time::elapsed(start);
    printf("debug : Time : %g\n",elapsed.seconds());
  
  }

  /*
  // Slower : we fill twice a std::vector !
  std::vector<float> row(100);
  for(count=0;count<rown;count++) {
    for(icol=0;icol<colin;icol++) {
      row[icol] = 100*count+icol;
    }
    for(icol=colin;icol<100;icol++) {
      row[icol] = 100*count+icol;
    }
    tuple->fill(row);
  }
  */


  delete aida; // Will delete the trees.

  // A simple check to see if object creation/destruction balance is correct :
  Lib::Printer printer;
  Lib::Debug::balance(printer);

  if(verboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Exiting..." << std::endl;
  }

  return 0;
}
