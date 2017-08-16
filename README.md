# ProjectorSimulator

템플릿 코드

<br>Mac 64bit debug 또는 release 모드에서 실행한다
<br>실행하면 3차원 그리드내에 3D 오브젝트와 프로젝터 오브젝트가 있으며 free camera로 둘러보기가 가능</br>

- Projector.h, cpp 설명
<br>Projector 클래스는 프로젝터 기계를 의미
<br>XYZ좌표이동과 XYZ축회전 가능
<br>실제 프로젝터의 스펙을 멤버변수로 이용할 예정
<br>각 프로젝터마다 개별적으로 사용할 영상을 지정할 예정</br>

- 현 구현상황
<br>기본 제공된 ofEasyCam으로 free camera 구현
<br>projector 오브젝트는 마우스로 선택이 가능. 선택된 프로젝터의 숫자가 좌측상단에 표시
<br>빔 프로젝터를 통해 영상을 쏜다는 느낌으로 시뮬레이션
<br>그림자 매핑을 통해 세부적인 텍스쳐 영역 처리</br>

- 테스트 방법
<br>OpenGL과 GLSL을 이용하여 쉐이더를 통한 Projection Texturing 테스트
<br>테스트용도로 이용할 영상파일을 ($프로젝트)/bin/data 경로에 넣은후
<br>Projector.cpp에서 void Projector::setup() 함수에서 주석처리된
<br>videoPlayer.load("*.*"); 코드에서 *.* 를 테스트할 영상파일로 고쳐주고 컴파일(느리다면 Release 모드로 할것)
<br>모델링에 영상이 비춰지는것을 확인할 수 있다.
<br>3차원 내에 있는 프로젝터 오브젝트를 마우스로 클릭하면 좌측에 있는 패널을 통해 XYZ좌표 이동 및 XYZ축 회전이 가능</br>


- TODO
1. 도량 통일 : 좌표계에서의 1씩 이동을 실세계의 몇cm,m로 환산할지?
2. 3D 모델링을 File Dialog로 불러오기
3. 3D 모델링을 관리할 클래스 구현(Projector 클래스처럼, Scale, Translate, Rotate 기능필수) -> 모델링을 움직일 필요성이 있는지의 이유로 보류됨
4. Projector 객체별로 영상을 File Dialog로 불러오기
5. Projector 객체의 비디오 플레이백 기능 구현(GUI 패널에서 버튼 클릭?)
6. easyCam 이용을 보다 쉽게(마우스 이동을 포함해서 키보드로까지)
7. Projector 및 3D 모델링에 쓰일 GIZMO 구현
8. 모델링에 영상을 비출 방법 모색
   <br>- Projection Texturing 테스트중
   <br>- Shadow Mapping 테스트중 : 단순 그림자표현은 완료, 추가적인 그림자 처리 고려
   <br>- 다수의 프로젝터를 동시에 구동했을시에도 잘 되는지 테스트 및 구현 필요</br>
