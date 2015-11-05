import xml.etree.ElementTree
import os;

class Defines:
    def __init__(self, name, desc):
        self.name = name
        self.description = desc
        
class EffectTechnique:
    def __init__(self, node):
        self.name = node.get('name')
        self.file = node.get('file')
        self.defines = []
        self.compile_effect_str = ''
        for define_node in node.findall('define'):
            self.defines.append( Defines(define_node.get('name'), define_node.get('description')) )
    def CompileEffect( self ):
        if not os.path.exists("fx"):
            os.makedirs("fx")
        fileNameOutput = "fx/" + self.name + ".o"
        self.compile_effect_str = 'fxc /LD /Tfx_2_0 /Fo' + fileNameOutput + ' ' + self.file
        for define in self.defines:
            self.compile_effect_str += ' /D' + define.name + '=' + define.description
        self.compile_effect_str += ' /DTECHNIQUE_NAME=' + self.name
        os.system(self.compile_effect_str)

class EffectsPool:
    def __init__(self):
        print("Initing Effects Pool")
        
    def CreateEffect( self, node ):
        if node.tag == 'technique' :
            lEffect = EffectTechnique(node)
            lEffect.CompileEffect()
        else :
            self.ParseXMLEffects(node.get('file'))
            
    def ParseXMLEffects( self, file ):
        print(file)
        root = xml.etree.ElementTree.parse(file).getroot()
        for child in root:
            self.CreateEffect( child )

lPool = EffectsPool()
lPool.ParseXMLEffects("../CommonData/effects/effects.xml")