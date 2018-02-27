type state = {
  input: string,
  formError: bool
};

type action =
  | RaiseFormError
  | UpdateInput(string);

let component = ReasonReact.reducerComponent("Form");

[%bs.raw {| require('./form.css') |}];

let make = (~onSubmit, _children) => {
  ...component,
  initialState: () => {input: "", formError: false},
  reducer: (action, state) =>
    switch action {
    | RaiseFormError => ReasonReact.Update({ ...state, formError: true })
    | UpdateInput(input) => ReasonReact.Update({...state, input})
    },
  render: self =>
    <form
      onSubmit=(
        event => {
          ReactEventRe.Form.preventDefault(event);
          switch self.state.input {
          | "" => self.send(RaiseFormError)
          | _ =>
            self.send(UpdateInput(""));
            onSubmit(self.state.input);
          };
        }
      )>
      <div>
        <input
          _type="text"
          placeholder="Add a todo"
          className="App-input"
          value=self.state.input
          onChange=(
            event =>
              self.send(
                UpdateInput(
                  ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value
                )
              )
          )
        />
        <button className="App-input-submit">
          (ReasonReact.stringToElement("Add"))
        </button>
      </div>
      (
        self.state.formError ?
          <p className="App-error">
            (ReasonReact.stringToElement("Field required"))
          </p> :
          ReasonReact.nullElement
      )
    </form>
};